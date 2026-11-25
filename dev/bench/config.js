window.ADAPTER_CONFIG = {
  // Path by default data.js relatively index.html
  dataUrl: "./data.js",

  // Project metadata for ASV head
  project: "thread_safe_lru_cache",
  branch: "master",
  machine: "github-runner",

  // Slowdown threshold for the Regressions tab (1.10 = warn if the slowdown exceeds 10%)
  regressionThreshold: 1.10,

  // ==========================================================================
  // 1. Filtering (works based on the original test names from data.js)
  // Can mix exact strings ("insert<lru_cache>") and regular expressions (/naive_rr/)
  filters: {
    include: [/.*/],
    exclude: [
      // Example:
      // /naive_rr_cache/,
      // "cache_fixture<thread_safe_dm_cache_std>/many_shards/threads:4"
    ]
  },

  // ==========================================================================
  // 2. Renaming (applied sequentially from top to bottom)
  // Important: avoid using parentheses (...) in new names — within ASV, they
  // are reserved for parameter lists; use square brackets [...] instead.
  renames: [
    { match: /^(?:queue_fixture\/)?perf_queue_/g, replace: "[stagequeue] " }
  ],

  // ==========================================================================
  // 3. Grouping and parameter axes (based on the renamed names)
  // Checked from top to bottom until the first match is found.
  // - section: a major section heading on the main page (Benchmark Grid)
  // - chart: the name of an individual chart within a section
  // - params: a dictionary of parameter axes that can be toggled within a chart
  groups: [
    {
      match: /^\[([^\]]+)\]\s+([^/]+)(?:\/iterations:\d+)?(?:\/real_time)?(?:\/threads:(\d+))?$/,
      section: "Queue Benchmarks",
      chart: "$2",
      params: {
        queue_type: "$1",
        threads: (m) => `threads:${m.at(3) || 1}`
      },
      metrics: [
        {
          section: "Operation Time",
          unit: (b) => b.unit || "ns/iter",
          getValue: (b) => Number(b.value)
        },
        {
          section: "CPU Time",
          unit: "ns",
          getValue: (b, extra) => extra.cpu ?? null
        },
        {
          section: "Speed",
          unit: "items/s",
          // 1 sec = 1e9 nano secs; convert ns/iter to items/sec
          getValue: (b, extra) => Number(b.value) > 0 ? (1e9 / Number(b.value)) * (Number(extra.threads) || 1) : null
        }
      ]
    },
    {
      // Default rule
      match: /^(.*)$/,
      section: "Other",
      chart: "$1",
      params: {
        variant: "default"
      }
    }
  ]
};
