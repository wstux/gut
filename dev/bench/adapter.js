(function () {
  if (window.__asvAdapterInstalled) return;
  window.__asvAdapterInstalled = true;

  const sanitizeFilename = (s) =>
    s.replace(/[^a-zA-Z0-9 _().-]/g, (c) => "_" + c.charCodeAt(0).toString(16).padStart(2, "0"));

  // Extra field parsing ("iterations: 212150\ncpu: 3.29 ns\nthreads: 1")
  function parseExtra(extraStr = "") {
    const res = {};
    if (!extraStr) return res;
    for (const line of String(extraStr).split("\n")) {
      const idx = line.indexOf(":");
      if (idx === -1) continue;
      const key = line.slice(0, idx).trim();
      const rawVal = line.slice(idx + 1).trim();
      const numVal = parseFloat(rawVal);
      res[key] = Number.isNaN(numVal) ? rawVal : numVal;
    }
    return res;
  }

  // 1. Name processing pipeline (filters -> renames -> groups)
  function matchesRule(str, rule) {
    return typeof rule === "string" ? str === rule : rule.test(str);
  }

  function interpolate(template, match) {
    if (typeof template === "function") return template(match);
    return String(template).replace(/\$(\d+)/g, (_, idx) => match.at(Number(idx)) ?? "");
  }

  function processBenchmarkItem(benchObj, cfg) {
    const rawName = benchObj.name;
    const { include = [], exclude = [] } = cfg.filters || {};

    if (include.length > 0 && !include.some((r) => matchesRule(rawName, r))) return [];
    if (exclude.some((r) => matchesRule(rawName, r))) return [];

    let renamed = rawName;
    for (const rule of (cfg.renames || [])) {
      renamed = renamed.replace(rule.match, rule.replace);
    }

    const extra = parseExtra(benchObj.extra);

    for (const grp of (cfg.groups || [])) {
      const m = typeof grp.match === "string"
        ? (renamed === grp.match ? [renamed] : null)
        : renamed.match(grp.match);

      if (m) {
        const baseSection = interpolate(grp.section || "Benchmarks", m);
        const baseChart = interpolate(grp.chart || renamed, m);
        const params = {};
        for (const [pk, pvTpl] of Object.entries(grp.params || {})) {
          params[pk] = interpolate(pvTpl, m);
        }

        const metricList = grp.metrics && grp.metrics.length > 0
          ? grp.metrics
          : [{ section: baseSection, chart: baseChart, unit: benchObj.unit || "ns", getValue: (b) => Number(b.value) }];

        const results = [];
        for (const metric of metricList) {
          const val = metric.getValue ? metric.getValue(benchObj, extra) : Number(benchObj.value);
          if (val === null || val === undefined || Number.isNaN(val)) continue;

          const sec = metric.section ? interpolate(metric.section, m) : baseSection;
          const chr = metric.chart ? interpolate(metric.chart, m) : baseChart;
          const unit = typeof metric.unit === "function" ? metric.unit(benchObj, extra) : (metric.unit || benchObj.unit || "ns");

          results.push({
            benchName: `${sec}.${chr}`,
            unit,
            params,
            value: Number(val)
          });
        }
        return results;
      }
    }

    return [{
      benchName: `Benchmarks.${renamed}`,
      unit: benchObj.unit || "ns",
      params: { variant: "default" },
      value: Number(benchObj.value)
    }];
  }

  // 2. Data normalizing
  function normalizeBenchmarkData(raw, cfg) {
    const runs = (raw?.entries?.Benchmark || []).slice().sort((a, b) => a.date - b.date);
    const revisionToHash = {};
    const revisionToDate = {};
    const grouped = new Map();

    runs.forEach((run, revIdx) => {
      revisionToHash[String(revIdx)] = run.commit.id;
      revisionToDate[String(revIdx)] = Number(run.date);

      for (const b of (run.benches || [])) {
        const items = processBenchmarkItem(b, cfg);

        for (const { benchName, unit, params, value } of items) {
          if (!grouped.has(benchName)) {
            grouped.set(benchName, {
              name: benchName,
              unit,
              paramSets: {},
              points: []
            });
          }

          const entry = grouped.get(benchName);
          for (const [pk, pv] of Object.entries(params)) {
            entry.paramSets[pk] = entry.paramSets[pk] || new Set();
            entry.paramSets[pk].add(pv);
          }
          entry.points.push({ revIdx, params, value });
        }
      }
    });

    const benchmarks = [];
    for (const entry of grouped.values()) {
      const paramNames = Object.keys(entry.paramSets);
      const paramValues = paramNames.map((k) => Array.from(entry.paramSets[k]).sort());
      const combos = paramValues.reduce((acc, list) => acc.flatMap((a) => list.map((v) => [...a, v])), [[]]);
      const comboIndex = new Map(combos.map((c, idx) => [JSON.stringify(c), idx]));

      const byRev = new Map();
      for (const pt of entry.points) {
        const tuple = paramNames.map((k) => pt.params[k]);
        const idx = comboIndex.get(JSON.stringify(tuple));
        if (!byRev.has(pt.revIdx)) byRev.set(pt.revIdx, Array(combos.length).fill(null));
        byRev.get(pt.revIdx)[idx] = pt.value;
      }

      const sortedRevs = Array.from(byRev.entries()).sort((a, b) => a.at(0) - b.at(0));

      benchmarks.push({
        name: entry.name,
        unit: entry.unit,
        paramNames,
        paramValues,
        combos,
        sortedRevs
      });
    }

    return { revisionToHash, revisionToDate, benchmarks };
  }

  // 3. Isolated converters for ASV files
  const ASV_CONVERTERS = {
    toMetadata(bench) {
      return {
        name: bench.name,
        pretty_name: bench.name,
        unit: bench.unit,
        type: "time",
        code: "",
        param_names: bench.paramNames,
        params: bench.paramValues
      };
    },

    toDetailGraph(bench) {
      return bench.sortedRevs;
    },

    toSummaryGraph(bench) {
      return bench.sortedRevs
        .map(([rev, vals]) => {
          const valid = vals.filter((v) => v !== null);
          return valid.length ? [rev, valid.reduce((s, x) => s + x, 0) / valid.length] : null;
        })
        .filter(Boolean);
    },

    toSummaryRows(bench) {
      const rows = [];
      bench.combos.forEach((combo, idx) => {
        const validPoints = bench.sortedRevs
          .map(([rev, vals]) => ({ rev, val: vals.at(idx) }))
          .filter((p) => p.val !== null && p.val !== undefined);

        if (validPoints.length === 0) return;

        const lastPt = validPoints.at(-1);
        const prevPt = validPoints.length >= 2 ? validPoints.at(-2) : null;
        const argsFormatted = combo.map((v) => `'${v}'`).join(", ");

        rows.push({
          name: bench.name,
          pretty_name: combo.length ? `${bench.name} (${argsFormatted})` : bench.name,
          idx: combo.length ? idx : null,
          last_value: lastPt.val,
          last_err: null,
          prev_value: prevPt ? prevPt.val : null,
          change_rev: prevPt ? [prevPt.rev, lastPt.rev] : null
        });
      });
      return rows;
    },

    toRegressions(bench, { graphPath, envSingle, threshold }) {
      const items = [];
      bench.combos.forEach((combo, idx) => {
        const validPoints = bench.sortedRevs
          .map(([rev, vals]) => ({ rev, val: vals.at(idx) }))
          .filter((p) => p.val !== null && p.val !== undefined);

        if (validPoints.length < 2) return;

        const lastPt = validPoints.at(-1);
        const prevPt = validPoints.at(-2);
        const bestVal = Math.min(...validPoints.map((p) => p.val));
        const argsFormatted = combo.map((v) => `'${v}'`).join(", ");
        const fullPrettyName = combo.length ? `${bench.name}(${argsFormatted})` : bench.name;

        if (lastPt.val > prevPt.val * threshold) {
          items.push([
            fullPrettyName,
            graphPath,
            envSingle,
            combo.length ? idx : null,
            lastPt.val,
            bestVal,
            [[prevPt.rev, lastPt.rev, prevPt.val, lastPt.val]]
          ]);
        }
      });
      return items;
    }
  };

  // 4. ASV virtual file system builder
  function buildVirtualAsvFiles(raw) {
    const cfg = window.ADAPTER_CONFIG;
    const { revisionToHash, revisionToDate, benchmarks } = normalizeBenchmarkData(raw, cfg);

    const envSingle = {
      arch: "x86_64",
      branch: cfg.branch,
      machine: cfg.machine,
      os: "Linux"
    };
    const envParams = Object.fromEntries(Object.entries(envSingle).map(([k, v]) => [k, [v]]));
    const baseGraphDir = `graphs/arch-x86_64/branch-${cfg.branch}/machine-${cfg.machine}/os-Linux`;

    const files = {};
    const benchmarksMeta = {};
    const summaryRows = [];
    const regressions = [];

    for (const bench of benchmarks) {
      const graphPath = `${baseGraphDir}/${sanitizeFilename(bench.name)}.json`;

      benchmarksMeta[bench.name] = ASV_CONVERTERS.toMetadata(bench);
      files[graphPath] = ASV_CONVERTERS.toDetailGraph(bench);
      files[`graphs/summary/${bench.name}.json`] = ASV_CONVERTERS.toSummaryGraph(bench);

      summaryRows.push(...ASV_CONVERTERS.toSummaryRows(bench));
      regressions.push(...ASV_CONVERTERS.toRegressions(bench, {
        graphPath,
        envSingle,
        threshold: cfg.regressionThreshold ?? 1.10
      }));
    }

    files[`${baseGraphDir}/summary.json`] = summaryRows;
    files["regressions.json"] = { regressions };
    files["info.json"] = { timestamp: Date.now() };
    files["index.json"] = {
      project: cfg.project,
      project_url: raw.repoUrl || "",
      show_commit_url: (raw.repoUrl || "") + "/commit/",
      hash_length: 7,
      revision_to_hash: revisionToHash,
      revision_to_date: revisionToDate,
      params: envParams,
      graph_param_list: [envSingle],
      benchmarks: benchmarksMeta,
      branches: [cfg.branch],
      tags: {},
      machines: { [cfg.machine]: { arch: "x86_64", os: "Linux", cpu: "GitHub Actions", ram: "" } }
    };

    return files;
  }

  // 5. Autoloading config.js + data.js and catching $.ajax
  function loadScript(src) {
    return new Promise((resolve, reject) => {
      const script = document.createElement("script");
      script.src = src;
      script.onload = resolve;
      script.onerror = () => reject(new Error(`Не удалось загрузить скрипт: ${src}`));
      document.head.appendChild(script);
    });
  }

  let virtualFilesPromise = null;

  function getVirtualFiles() {
    if (!virtualFilesPromise) {
      virtualFilesPromise = (async () => {
        if (!window.ADAPTER_CONFIG) {
          await loadScript("./config.js");
        }
        if (!window.ADAPTER_CONFIG) {
          throw new Error("window.ADAPTER_CONFIG не найден в config.js");
        }
        if (!window.BENCHMARK_DATA) {
          await loadScript(window.ADAPTER_CONFIG.dataUrl);
        }
        if (!window.BENCHMARK_DATA) {
          throw new Error("window.BENCHMARK_DATA не найден после загрузки " + window.ADAPTER_CONFIG.dataUrl);
        }
        return buildVirtualAsvFiles(window.BENCHMARK_DATA);
      })();
    }
    return virtualFilesPromise;
  }

  const origAjax = $.ajax;

  $.ajax = function (urlOrOptions, options) {
    const opts = typeof urlOrOptions === "string" ? { ...options, url: urlOrOptions } : { ...urlOrOptions };
    const cleanPath = decodeURIComponent((opts.url || "").split("?").at(0).replace(/^\/+/, ""));

    if (cleanPath.endsWith(".json")) {
      const dfd = $.Deferred();
      getVirtualFiles()
        .then((files) => {
          if (Object.prototype.hasOwnProperty.call(files, cleanPath)) {
            const data = files[cleanPath];
            if (opts.success) opts.success(data, "success", {});
            dfd.resolve(data, "success", {});
          } else {
            origAjax.apply(this, arguments).done(dfd.resolve).fail(dfd.reject);
          }
        })
        .catch((err) => {
          console.error("[ASV Adapter]", err);
          if (opts.error) opts.error({}, "error", err.message);
          dfd.reject({}, "error", err.message);
        });
      return dfd.promise();
    }

    return origAjax.apply(this, arguments);
  };
})();
