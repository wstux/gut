window.BENCHMARK_DATA = {
  "lastUpdate": 1795341931458,
  "repoUrl": "https://github.com/wstux/thread_safe_lru_cache",
  "entries": {
    "Benchmark": [
      {
        "commit": {
          "author": {
            "email": "wstux1@gmail.com",
            "name": "wstux",
            "username": "wstux"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "7c7375d251fe101665014f0cf7b793b69d292250",
          "message": "* all: #40 Merge remote-tracking branch 'wstux/feature/update' into master;",
          "timestamp": "2026-11-19T14:07:35+03:00",
          "tree_id": "dd2fab3af84df4b541f5c76c2109b6ce8ccbe9ae",
          "url": "https://github.com/wstux/thread_safe_lru_cache/commit/7c7375d251fe101665014f0cf7b793b69d292250"
        },
        "date": 1795086710588,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "insert<fifo_cache_intrusive>",
            "value": 3.292640435953777,
            "unit": "ns/iter",
            "extra": "iterations: 212150569\ncpu: 3.292385565084202 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_intrusive>",
            "value": 3.2826617143709718,
            "unit": "ns/iter",
            "extra": "iterations: 211585648\ncpu: 3.282347430294516 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_intrusive>",
            "value": 3.28079509489239,
            "unit": "ns/iter",
            "extra": "iterations: 212652454\ncpu: 3.280479486025588 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_intrusive>",
            "value": 3.2723463773163095,
            "unit": "ns/iter",
            "extra": "iterations: 214174995\ncpu: 3.272070971683692 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_intrusive>",
            "value": 3.323559995061193,
            "unit": "ns/iter",
            "extra": "iterations: 214164387\ncpu: 3.3232368881199683 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_intrusive>",
            "value": 3.8574711593987288,
            "unit": "ns/iter",
            "extra": "iterations: 181815904\ncpu: 3.857060958759692 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_intrusive>",
            "value": 165.7922318052589,
            "unit": "ns/iter",
            "extra": "iterations: 4210914\ncpu: 165.78278160038417 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_intrusive>",
            "value": 168.86305492924933,
            "unit": "ns/iter",
            "extra": "iterations: 4157039\ncpu: 168.85028670647546 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_intrusive>",
            "value": 169.50907417333534,
            "unit": "ns/iter",
            "extra": "iterations: 4210907\ncpu: 169.49459795716226 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_intrusive>",
            "value": 168.863180172513,
            "unit": "ns/iter",
            "extra": "iterations: 4175981\ncpu: 168.85833460449152 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_intrusive>",
            "value": 3.3766122524286235,
            "unit": "ns/iter",
            "extra": "iterations: 203349903\ncpu: 3.3764122769215206 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_intrusive>",
            "value": 143.47358678172944,
            "unit": "ns/iter",
            "extra": "iterations: 5135194\ncpu: 143.45980794493855 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_intrusive>",
            "value": 3.3227802060599574,
            "unit": "ns/iter",
            "extra": "iterations: 210302527\ncpu: 3.3224244471394333 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_intrusive>",
            "value": 3.3224009434957895,
            "unit": "ns/iter",
            "extra": "iterations: 209505122\ncpu: 3.3222004137922734 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_intrusive>",
            "value": 3.298620827716305,
            "unit": "ns/iter",
            "extra": "iterations: 210331518\ncpu: 3.2984474347777004 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_intrusive>",
            "value": 3.2936725020031177,
            "unit": "ns/iter",
            "extra": "iterations: 212705417\ncpu: 3.2934060865972214 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_intrusive>",
            "value": 3.2729175936746557,
            "unit": "ns/iter",
            "extra": "iterations: 213956767\ncpu: 3.272723853599832 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_intrusive>",
            "value": 3.868980339075679,
            "unit": "ns/iter",
            "extra": "iterations: 181267062\ncpu: 3.8688087193689924 ns\nthreads: 1"
          },
          {
            "name": "insert<naive_rr_cache_intrusive>",
            "value": 3.273334409830884,
            "unit": "ns/iter",
            "extra": "iterations: 206038530\ncpu: 3.2731651259596943 ns\nthreads: 1"
          },
          {
            "name": "emplace<naive_rr_cache_intrusive>",
            "value": 3.276432816417669,
            "unit": "ns/iter",
            "extra": "iterations: 213347517\ncpu: 3.2763053858273916 ns\nthreads: 1"
          },
          {
            "name": "update_insert<naive_rr_cache_intrusive>",
            "value": 3.2740746796903535,
            "unit": "ns/iter",
            "extra": "iterations: 213568802\ncpu: 3.2739041538473486 ns\nthreads: 1"
          },
          {
            "name": "update<naive_rr_cache_intrusive>",
            "value": 3.3016555962042657,
            "unit": "ns/iter",
            "extra": "iterations: 214059261\ncpu: 3.3014065950643374 ns\nthreads: 1"
          },
          {
            "name": "find<naive_rr_cache_intrusive>",
            "value": 9.299999987888441e-10,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 5.640000004802914e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<naive_rr_cache_intrusive>",
            "value": 3.265989338517828,
            "unit": "ns/iter",
            "extra": "iterations: 214393078\ncpu: 3.2658309565386263 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_intrusive>",
            "value": 3.272533746833811,
            "unit": "ns/iter",
            "extra": "iterations: 212969387\ncpu: 3.272286373252335 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_intrusive>",
            "value": 3.2752295710689534,
            "unit": "ns/iter",
            "extra": "iterations: 213236255\ncpu: 3.2750297832795976 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_intrusive>",
            "value": 3.340031920982416,
            "unit": "ns/iter",
            "extra": "iterations: 208702850\ncpu: 3.3396781165182885 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_intrusive>",
            "value": 3.403431345443164,
            "unit": "ns/iter",
            "extra": "iterations: 209978917\ncpu: 3.4030292050701423 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_intrusive>",
            "value": 1.4540000279339438e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 6.959999971911657e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_intrusive>",
            "value": 56.82850135982182,
            "unit": "ns/iter",
            "extra": "iterations: 13516113\ncpu: 56.822735131024565 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_intrusive>",
            "value": 35.38150319982125,
            "unit": "ns/iter",
            "extra": "iterations: 20142219\ncpu: 35.378001053409314 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_intrusive>",
            "value": 35.08806606509443,
            "unit": "ns/iter",
            "extra": "iterations: 19970848\ncpu: 35.08484301718191 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_intrusive>",
            "value": 3.4674752289278454,
            "unit": "ns/iter",
            "extra": "iterations: 208133302\ncpu: 3.4672582141612023 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_intrusive>",
            "value": 3.340327630092105,
            "unit": "ns/iter",
            "extra": "iterations: 211881392\ncpu: 3.3402019512879195 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_intrusive>",
            "value": 3.292020305651635,
            "unit": "ns/iter",
            "extra": "iterations: 210551235\ncpu: 3.2918213137054004 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_intrusive>",
            "value": 4.431687964066773,
            "unit": "ns/iter",
            "extra": "iterations: 159873818\ncpu: 4.431512913515348 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert/threads:4",
            "value": 112.27383747575266,
            "unit": "ns/iter",
            "extra": "iterations: 4800760\ncpu: 111.91382135328573 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/emplace/threads:4",
            "value": 111.69946674916265,
            "unit": "ns/iter",
            "extra": "iterations: 5534356\ncpu: 110.89015415705097 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update_insert/threads:4",
            "value": 114.4786563198397,
            "unit": "ns/iter",
            "extra": "iterations: 5952488\ncpu: 114.22535013930312 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update/threads:4",
            "value": 113.34936700769737,
            "unit": "ns/iter",
            "extra": "iterations: 5569736\ncpu: 113.24137104523449 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/find/threads:4",
            "value": 52.648435410614965,
            "unit": "ns/iter",
            "extra": "iterations: 13416044\ncpu: 52.43497166526885 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert_overflow/threads:4",
            "value": 146.54750958452175,
            "unit": "ns/iter",
            "extra": "iterations: 4101404\ncpu: 146.3335925941458 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request/threads:4",
            "value": 142.91914713181836,
            "unit": "ns/iter",
            "extra": "iterations: 4506816\ncpu: 142.7967185702722 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request_hot/threads:4",
            "value": 145.4492354050088,
            "unit": "ns/iter",
            "extra": "iterations: 4547244\ncpu: 127.28691136873239 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/many_shards/threads:4",
            "value": 85.73882881997413,
            "unit": "ns/iter",
            "extra": "iterations: 7815468\ncpu: 85.21461069253947 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert/threads:4",
            "value": 552.7008971313275,
            "unit": "ns/iter",
            "extra": "iterations: 1884228\ncpu: 551.3629735891836 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/emplace/threads:4",
            "value": 529.6698891264521,
            "unit": "ns/iter",
            "extra": "iterations: 1805660\ncpu: 529.2508539813698 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update_insert/threads:4",
            "value": 575.0040211702116,
            "unit": "ns/iter",
            "extra": "iterations: 1748496\ncpu: 568.3154511076948 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update/threads:4",
            "value": 514.9824184418569,
            "unit": "ns/iter",
            "extra": "iterations: 1529216\ncpu: 514.3244662624508 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/find/threads:4",
            "value": 111.32198621151166,
            "unit": "ns/iter",
            "extra": "iterations: 6234180\ncpu: 111.27405560955881 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert_overflow/threads:4",
            "value": 445.46453156208975,
            "unit": "ns/iter",
            "extra": "iterations: 1838536\ncpu: 445.1809891130769 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request/threads:4",
            "value": 553.1205021433309,
            "unit": "ns/iter",
            "extra": "iterations: 1647896\ncpu: 549.6322595600694 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request_hot/threads:4",
            "value": 552.8276656410894,
            "unit": "ns/iter",
            "extra": "iterations: 1553660\ncpu: 548.8470991079128 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/many_shards/threads:4",
            "value": 464.43159226411365,
            "unit": "ns/iter",
            "extra": "iterations: 1907372\ncpu: 463.8502043649585 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert/threads:4",
            "value": 130.21447450000778,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 129.77621349999998 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/emplace/threads:4",
            "value": 127.35118957240837,
            "unit": "ns/iter",
            "extra": "iterations: 5585032\ncpu: 126.568384388845 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update_insert/threads:4",
            "value": 128.651636105825,
            "unit": "ns/iter",
            "extra": "iterations: 5441824\ncpu: 128.46403264787682 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update/threads:4",
            "value": 128.17513213986132,
            "unit": "ns/iter",
            "extra": "iterations: 5123360\ncpu: 127.25713613722247 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/find/threads:4",
            "value": 56.45158049267538,
            "unit": "ns/iter",
            "extra": "iterations: 12278956\ncpu: 56.32703488798231 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert_overflow/threads:4",
            "value": 152.9774074780089,
            "unit": "ns/iter",
            "extra": "iterations: 4154472\ncpu: 152.79009847701468 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request/threads:4",
            "value": 157.72740404599537,
            "unit": "ns/iter",
            "extra": "iterations: 4359276\ncpu: 157.61109069487694 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request_hot/threads:4",
            "value": 156.1589289220367,
            "unit": "ns/iter",
            "extra": "iterations: 4339292\ncpu: 156.0814655939264 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/many_shards/threads:4",
            "value": 106.33303332560368,
            "unit": "ns/iter",
            "extra": "iterations: 6466500\ncpu: 106.18272249284786 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/insert/threads:4",
            "value": 77.72510391536623,
            "unit": "ns/iter",
            "extra": "iterations: 8966912\ncpu: 77.6140452811403 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/emplace/threads:4",
            "value": 77.73018257224626,
            "unit": "ns/iter",
            "extra": "iterations: 8907816\ncpu: 77.59686257551787 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/update_insert/threads:4",
            "value": 78.15635787164884,
            "unit": "ns/iter",
            "extra": "iterations: 8858908\ncpu: 77.97542733257855 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/update/threads:4",
            "value": 79.62014205267528,
            "unit": "ns/iter",
            "extra": "iterations: 8859812\ncpu: 79.23276193670922 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/find/threads:4",
            "value": 48.342313601509844,
            "unit": "ns/iter",
            "extra": "iterations: 14517124\ncpu: 48.32835642927625 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/insert_overflow/threads:4",
            "value": 62.44056749813341,
            "unit": "ns/iter",
            "extra": "iterations: 11160072\ncpu: 62.373473665761296 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/request/threads:4",
            "value": 88.88287697363948,
            "unit": "ns/iter",
            "extra": "iterations: 7863680\ncpu: 88.36977814458385 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/request_hot/threads:4",
            "value": 88.62570391875428,
            "unit": "ns/iter",
            "extra": "iterations: 7896508\ncpu: 88.50837534768527 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_intrusive>/many_shards/threads:4",
            "value": 71.62942131565798,
            "unit": "ns/iter",
            "extra": "iterations: 9495816\ncpu: 71.61388120831323 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert/threads:4",
            "value": 95.9372861903073,
            "unit": "ns/iter",
            "extra": "iterations: 7118480\ncpu: 95.67664627841896 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/emplace/threads:4",
            "value": 94.96025957172398,
            "unit": "ns/iter",
            "extra": "iterations: 7009084\ncpu: 94.75980770097779 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update_insert/threads:4",
            "value": 98.59580917787856,
            "unit": "ns/iter",
            "extra": "iterations: 6812124\ncpu: 97.60861149914476 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update/threads:4",
            "value": 96.33085541024778,
            "unit": "ns/iter",
            "extra": "iterations: 7061828\ncpu: 96.08341522903125 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/find/threads:4",
            "value": 51.30758887300605,
            "unit": "ns/iter",
            "extra": "iterations: 13724752\ncpu: 51.25505684911469 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert_overflow/threads:4",
            "value": 134.10612434725851,
            "unit": "ns/iter",
            "extra": "iterations: 4154108\ncpu: 133.02123873524747 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request/threads:4",
            "value": 126.82132131682965,
            "unit": "ns/iter",
            "extra": "iterations: 4681784\ncpu: 126.66000140117558 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request_hot/threads:4",
            "value": 126.37373054507188,
            "unit": "ns/iter",
            "extra": "iterations: 4780792\ncpu: 126.24618619676433 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/many_shards/threads:4",
            "value": 77.56891822027376,
            "unit": "ns/iter",
            "extra": "iterations: 8681028\ncpu: 77.46027083428363 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert/threads:4",
            "value": 251.9410708611686,
            "unit": "ns/iter",
            "extra": "iterations: 3562516\ncpu: 250.5042380160536 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/emplace/threads:4",
            "value": 218.81335097191547,
            "unit": "ns/iter",
            "extra": "iterations: 3564160\ncpu: 218.56323200978622 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update_insert/threads:4",
            "value": 149.07818497228428,
            "unit": "ns/iter",
            "extra": "iterations: 4187352\ncpu: 148.89792737749272 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update/threads:4",
            "value": 145.13410527846946,
            "unit": "ns/iter",
            "extra": "iterations: 4264120\ncpu: 144.92377864600445 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/find/threads:4",
            "value": 76.71608415612455,
            "unit": "ns/iter",
            "extra": "iterations: 9182552\ncpu: 76.431667035482 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert_overflow/threads:4",
            "value": 250.8526432318919,
            "unit": "ns/iter",
            "extra": "iterations: 3050508\ncpu: 250.43434175553696 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request/threads:4",
            "value": 248.70666111724438,
            "unit": "ns/iter",
            "extra": "iterations: 3178716\ncpu: 248.50354010864734 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request_hot/threads:4",
            "value": 258.1797219893976,
            "unit": "ns/iter",
            "extra": "iterations: 2820612\ncpu: 256.86108085762925 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/many_shards/threads:4",
            "value": 195.31045720293253,
            "unit": "ns/iter",
            "extra": "iterations: 3607676\ncpu: 194.15492328024985 ns\nthreads: 4"
          },
          {
            "name": "insert<fifo_cache_std>",
            "value": 3.316528559952148,
            "unit": "ns/iter",
            "extra": "iterations: 210207496\ncpu: 3.316385996054109 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_std>",
            "value": 3.3116748249255235,
            "unit": "ns/iter",
            "extra": "iterations: 209935525\ncpu: 3.3116262004727406 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_std>",
            "value": 3.394783022076363,
            "unit": "ns/iter",
            "extra": "iterations: 207615638\ncpu: 3.3946729195803647 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_std>",
            "value": 3.3244081894816304,
            "unit": "ns/iter",
            "extra": "iterations: 210593207\ncpu: 3.3241904189245783 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_std>",
            "value": 1.8129999830307498e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 6.930000004956583e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_std>",
            "value": 20.005510422662447,
            "unit": "ns/iter",
            "extra": "iterations: 35046132\ncpu: 20.00424092450487 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_std>",
            "value": 34.59992623589629,
            "unit": "ns/iter",
            "extra": "iterations: 20042269\ncpu: 34.59754042818207 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_std>",
            "value": 34.463461878736545,
            "unit": "ns/iter",
            "extra": "iterations: 20034815\ncpu: 34.4608684931705 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_std>",
            "value": 34.37819041208042,
            "unit": "ns/iter",
            "extra": "iterations: 20131412\ncpu: 34.374768347098566 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_std>",
            "value": 33.88567599915451,
            "unit": "ns/iter",
            "extra": "iterations: 20522681\ncpu: 33.883311980535055 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_std>",
            "value": 4.914823383498761,
            "unit": "ns/iter",
            "extra": "iterations: 135336052\ncpu: 4.914382931755686 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_std>",
            "value": 59.62296370757524,
            "unit": "ns/iter",
            "extra": "iterations: 11631188\ncpu: 59.619830493669255 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_std>",
            "value": 8.115881715291676,
            "unit": "ns/iter",
            "extra": "iterations: 130932123\ncpu: 8.115373963652909 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_std>",
            "value": 7.251481025450496,
            "unit": "ns/iter",
            "extra": "iterations: 121920761\ncpu: 7.251225105132015 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_std>",
            "value": 5.3636532468212375,
            "unit": "ns/iter",
            "extra": "iterations: 124696420\ncpu: 5.363219890354516 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_std>",
            "value": 5.323256857128785,
            "unit": "ns/iter",
            "extra": "iterations: 131189347\ncpu: 5.3230341789871085 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_std>",
            "value": 4.888087278670225,
            "unit": "ns/iter",
            "extra": "iterations: 143082840\ncpu: 4.887712411914661 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_std>",
            "value": 19.985091655339513,
            "unit": "ns/iter",
            "extra": "iterations: 34607531\ncpu: 19.984066040423432 ns\nthreads: 1"
          },
          {
            "name": "insert<naive_rr_cache_std>",
            "value": 1.3240163725318028,
            "unit": "ns/iter",
            "extra": "iterations: 526002213\ncpu: 1.3239740000865794 ns\nthreads: 1"
          },
          {
            "name": "emplace<naive_rr_cache_std>",
            "value": 1.349198112731963,
            "unit": "ns/iter",
            "extra": "iterations: 520825017\ncpu: 1.3491319139149565 ns\nthreads: 1"
          },
          {
            "name": "update_insert<naive_rr_cache_std>",
            "value": 1.3920866980106161,
            "unit": "ns/iter",
            "extra": "iterations: 499691558\ncpu: 1.3920220141081532 ns\nthreads: 1"
          },
          {
            "name": "update<naive_rr_cache_std>",
            "value": 1.4052400139944177,
            "unit": "ns/iter",
            "extra": "iterations: 501047784\ncpu: 1.4051756369009292 ns\nthreads: 1"
          },
          {
            "name": "find<naive_rr_cache_std>",
            "value": 8.590000106778462e-10,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 4.3699999707769165e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<naive_rr_cache_std>",
            "value": 1.4069152376631935,
            "unit": "ns/iter",
            "extra": "iterations: 502275145\ncpu: 1.4069063222310147 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_std>",
            "value": 3.2774098046831632,
            "unit": "ns/iter",
            "extra": "iterations: 212565360\ncpu: 3.2772888630584034 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_std>",
            "value": 3.2800890547514205,
            "unit": "ns/iter",
            "extra": "iterations: 213265656\ncpu: 3.279985451572197 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_std>",
            "value": 3.3451988438778373,
            "unit": "ns/iter",
            "extra": "iterations: 210526522\ncpu: 3.3450591322645833 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_std>",
            "value": 3.3105806545825356,
            "unit": "ns/iter",
            "extra": "iterations: 209928146\ncpu: 3.3104521153633266 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_std>",
            "value": 1.0150000093744893e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 6.399999996631322e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_std>",
            "value": 20.308766763703872,
            "unit": "ns/iter",
            "extra": "iterations: 34535401\ncpu: 20.307431959455098 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_std>",
            "value": 36.50763637928814,
            "unit": "ns/iter",
            "extra": "iterations: 19146181\ncpu: 36.50564538170826 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_std>",
            "value": 37.014448319295234,
            "unit": "ns/iter",
            "extra": "iterations: 19133921\ncpu: 37.013222120024494 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_std>",
            "value": 5.437245291457618,
            "unit": "ns/iter",
            "extra": "iterations: 116994751\ncpu: 5.437033683673529 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_std>",
            "value": 5.523140078433837,
            "unit": "ns/iter",
            "extra": "iterations: 129268771\ncpu: 5.52276045078204 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_std>",
            "value": 4.922109584338237,
            "unit": "ns/iter",
            "extra": "iterations: 135102463\ncpu: 4.921884170239006 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_std>",
            "value": 20.92003869358332,
            "unit": "ns/iter",
            "extra": "iterations: 33182762\ncpu: 20.918409594716625 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert/threads:4",
            "value": 117.93452093753555,
            "unit": "ns/iter",
            "extra": "iterations: 6134816\ncpu: 117.83614667497773 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/emplace/threads:4",
            "value": 115.49191227268237,
            "unit": "ns/iter",
            "extra": "iterations: 5083752\ncpu: 115.38475873724761 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update_insert/threads:4",
            "value": 165.8563751664744,
            "unit": "ns/iter",
            "extra": "iterations: 4619048\ncpu: 156.96332231230335 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update/threads:4",
            "value": 149.20623441609254,
            "unit": "ns/iter",
            "extra": "iterations: 4692340\ncpu: 140.06893682043503 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/find/threads:4",
            "value": 39.45797066588513,
            "unit": "ns/iter",
            "extra": "iterations: 17843252\ncpu: 39.42827731178151 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert_overflow/threads:4",
            "value": 111.60234328933863,
            "unit": "ns/iter",
            "extra": "iterations: 4687684\ncpu: 111.4750887645157 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request/threads:4",
            "value": 123.48831061160323,
            "unit": "ns/iter",
            "extra": "iterations: 4836780\ncpu: 122.83403338584763 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request_hot/threads:4",
            "value": 126.4607782963383,
            "unit": "ns/iter",
            "extra": "iterations: 5050724\ncpu: 126.32002501027576 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/many_shards/threads:4",
            "value": 133.14460702587232,
            "unit": "ns/iter",
            "extra": "iterations: 4510424\ncpu: 133.04252504864291 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert/threads:4",
            "value": 766.6093931547106,
            "unit": "ns/iter",
            "extra": "iterations: 1745420\ncpu: 674.420017531597 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/emplace/threads:4",
            "value": 657.1200976714264,
            "unit": "ns/iter",
            "extra": "iterations: 1706128\ncpu: 656.7054980634514 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update_insert/threads:4",
            "value": 660.5910735245127,
            "unit": "ns/iter",
            "extra": "iterations: 1765288\ncpu: 659.0684188642304 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update/threads:4",
            "value": 676.4819779494629,
            "unit": "ns/iter",
            "extra": "iterations: 1273440\ncpu: 672.3834126460611 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/find/threads:4",
            "value": 185.63607900170456,
            "unit": "ns/iter",
            "extra": "iterations: 4234136\ncpu: 185.60539245787103 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert_overflow/threads:4",
            "value": 607.0563369076539,
            "unit": "ns/iter",
            "extra": "iterations: 1659232\ncpu: 601.1564404495575 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request/threads:4",
            "value": 632.1218681306577,
            "unit": "ns/iter",
            "extra": "iterations: 1728680\ncpu: 630.8966448388366 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request_hot/threads:4",
            "value": 654.7717018811053,
            "unit": "ns/iter",
            "extra": "iterations: 1446188\ncpu: 654.1522616699901 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/many_shards/threads:4",
            "value": 699.2594476340475,
            "unit": "ns/iter",
            "extra": "iterations: 1581560\ncpu: 697.7547902071373 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert/threads:4",
            "value": 230.84451207655303,
            "unit": "ns/iter",
            "extra": "iterations: 4113760\ncpu: 229.72196384815837 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/emplace/threads:4",
            "value": 218.68872533061256,
            "unit": "ns/iter",
            "extra": "iterations: 4178056\ncpu: 218.4033359055026 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update_insert/threads:4",
            "value": 225.65875944697584,
            "unit": "ns/iter",
            "extra": "iterations: 4453272\ncpu: 225.36440936012897 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update/threads:4",
            "value": 200.52049978087453,
            "unit": "ns/iter",
            "extra": "iterations: 4070824\ncpu: 200.22893989030248 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/find/threads:4",
            "value": 50.364662353375536,
            "unit": "ns/iter",
            "extra": "iterations: 14247440\ncpu: 50.35689099234668 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert_overflow/threads:4",
            "value": 207.21464018886394,
            "unit": "ns/iter",
            "extra": "iterations: 3856412\ncpu: 206.75378823631908 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request/threads:4",
            "value": 212.0833851989698,
            "unit": "ns/iter",
            "extra": "iterations: 3727580\ncpu: 210.8864397276516 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request_hot/threads:4",
            "value": 188.1434098411376,
            "unit": "ns/iter",
            "extra": "iterations: 4138140\ncpu: 187.67611753106485 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/many_shards/threads:4",
            "value": 204.1707344707199,
            "unit": "ns/iter",
            "extra": "iterations: 4104180\ncpu: 203.7393462275045 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/insert/threads:4",
            "value": 77.96032283663111,
            "unit": "ns/iter",
            "extra": "iterations: 8972340\ncpu: 77.47367019083096 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/emplace/threads:4",
            "value": 78.26831903674382,
            "unit": "ns/iter",
            "extra": "iterations: 8896756\ncpu: 78.21055865756009 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/update_insert/threads:4",
            "value": 80.29268985218829,
            "unit": "ns/iter",
            "extra": "iterations: 8720152\ncpu: 80.14553198155248 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/update/threads:4",
            "value": 80.54762813503983,
            "unit": "ns/iter",
            "extra": "iterations: 8688100\ncpu: 77.06992069612463 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/find/threads:4",
            "value": 49.78434448308645,
            "unit": "ns/iter",
            "extra": "iterations: 14225720\ncpu: 49.7048938120531 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/insert_overflow/threads:4",
            "value": 63.62557308194214,
            "unit": "ns/iter",
            "extra": "iterations: 11058628\ncpu: 63.25097923539881 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/request/threads:4",
            "value": 96.49358403611888,
            "unit": "ns/iter",
            "extra": "iterations: 7246456\ncpu: 96.28636494860379 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/request_hot/threads:4",
            "value": 95.97336608293409,
            "unit": "ns/iter",
            "extra": "iterations: 7287212\ncpu: 95.37308191390616 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_naive_rr_cache_std>/many_shards/threads:4",
            "value": 73.52722905072409,
            "unit": "ns/iter",
            "extra": "iterations: 9155736\ncpu: 73.44647115207336 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert/threads:4",
            "value": 92.29194779135251,
            "unit": "ns/iter",
            "extra": "iterations: 7487188\ncpu: 92.06781771741281 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/emplace/threads:4",
            "value": 88.58680356405597,
            "unit": "ns/iter",
            "extra": "iterations: 7378568\ncpu: 88.26965706624924 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update_insert/threads:4",
            "value": 97.59461325032728,
            "unit": "ns/iter",
            "extra": "iterations: 7105992\ncpu: 97.48836953376814 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update/threads:4",
            "value": 95.33902447304078,
            "unit": "ns/iter",
            "extra": "iterations: 7304688\ncpu: 95.21458137568636 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/find/threads:4",
            "value": 38.2274912352694,
            "unit": "ns/iter",
            "extra": "iterations: 17690220\ncpu: 38.19101950117069 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert_overflow/threads:4",
            "value": 103.20114367915613,
            "unit": "ns/iter",
            "extra": "iterations: 5050892\ncpu: 103.07879063737643 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request/threads:4",
            "value": 88.76775653313342,
            "unit": "ns/iter",
            "extra": "iterations: 7708712\ncpu: 88.65085762705885 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request_hot/threads:4",
            "value": 87.82726369585485,
            "unit": "ns/iter",
            "extra": "iterations: 7692332\ncpu: 87.61360768099985 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/many_shards/threads:4",
            "value": 91.6169173638869,
            "unit": "ns/iter",
            "extra": "iterations: 7448560\ncpu: 91.56837724338659 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert/threads:4",
            "value": 288.94675240672234,
            "unit": "ns/iter",
            "extra": "iterations: 3206436\ncpu: 288.5731996521994 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/emplace/threads:4",
            "value": 283.33198787189605,
            "unit": "ns/iter",
            "extra": "iterations: 3322528\ncpu: 282.7604357284574 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update_insert/threads:4",
            "value": 205.96672847505002,
            "unit": "ns/iter",
            "extra": "iterations: 3790208\ncpu: 204.0256943682248 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update/threads:4",
            "value": 221.5759041232132,
            "unit": "ns/iter",
            "extra": "iterations: 3590108\ncpu: 221.3980554345437 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/find/threads:4",
            "value": 84.73703269192794,
            "unit": "ns/iter",
            "extra": "iterations: 9192116\ncpu: 84.7027792077472 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert_overflow/threads:4",
            "value": 277.4408853819832,
            "unit": "ns/iter",
            "extra": "iterations: 2852780\ncpu: 275.7134812358464 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request/threads:4",
            "value": 279.5153230514078,
            "unit": "ns/iter",
            "extra": "iterations: 3015000\ncpu: 279.1250749585402 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request_hot/threads:4",
            "value": 284.29155018679415,
            "unit": "ns/iter",
            "extra": "iterations: 2851708\ncpu: 283.8346012284572 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/many_shards/threads:4",
            "value": 277.418300722842,
            "unit": "ns/iter",
            "extra": "iterations: 2926256\ncpu: 277.0968524284955 ns\nthreads: 4"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wstux1@gmail.com",
            "name": "wstux",
            "username": "wstux"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "b40140f3b1704faf6f0f0c49d38445d10b0c8fe8",
          "message": "* all: #41 Merge remote-tracking branch 'wstux/feature/dm_cache' into master;",
          "timestamp": "2026-11-19T21:48:58+03:00",
          "tree_id": "7b73f756de02db9a9988c8aead4a3139dec9001a",
          "url": "https://github.com/wstux/thread_safe_lru_cache/commit/b40140f3b1704faf6f0f0c49d38445d10b0c8fe8"
        },
        "date": 1795114386499,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "insert<dm_cache_intrusive>",
            "value": 3.3217721950257695,
            "unit": "ns/iter",
            "extra": "iterations: 211403347\ncpu: 3.3214682026770364 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_intrusive>",
            "value": 3.3014763533197393,
            "unit": "ns/iter",
            "extra": "iterations: 210498934\ncpu: 3.3004057018169966 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_intrusive>",
            "value": 3.3183479278607115,
            "unit": "ns/iter",
            "extra": "iterations: 211522894\ncpu: 3.3170038085806457 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_intrusive>",
            "value": 3.297440669814414,
            "unit": "ns/iter",
            "extra": "iterations: 211735517\ncpu: 3.296109183231646 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_intrusive>",
            "value": 1.4940000028218491e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 6.7499999989451e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_intrusive>",
            "value": 3.305798465998443,
            "unit": "ns/iter",
            "extra": "iterations: 211349726\ncpu: 3.3043033280298633 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_intrusive>",
            "value": 3.3358343678310933,
            "unit": "ns/iter",
            "extra": "iterations: 210018961\ncpu: 3.335706750782376 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_intrusive>",
            "value": 3.327629995181878,
            "unit": "ns/iter",
            "extra": "iterations: 210586555\ncpu: 3.3274034897432085 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_intrusive>",
            "value": 3.3093585465774016,
            "unit": "ns/iter",
            "extra": "iterations: 210971873\ncpu: 3.3092829346023818 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_intrusive>",
            "value": 3.3287350304563317,
            "unit": "ns/iter",
            "extra": "iterations: 213144942\ncpu: 3.327908592829761 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_intrusive>",
            "value": 3.3054533738171563,
            "unit": "ns/iter",
            "extra": "iterations: 211300259\ncpu: 3.304524037521413 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_intrusive>",
            "value": 3.926247990932968,
            "unit": "ns/iter",
            "extra": "iterations: 178101955\ncpu: 3.925452615048506 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_intrusive>",
            "value": 131.8963456034564,
            "unit": "ns/iter",
            "extra": "iterations: 5480385\ncpu: 131.68040292789635 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_intrusive>",
            "value": 131.23477565782338,
            "unit": "ns/iter",
            "extra": "iterations: 5390627\ncpu: 131.22401568500268 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_intrusive>",
            "value": 128.97332257557562,
            "unit": "ns/iter",
            "extra": "iterations: 5503005\ncpu: 128.9670427339246 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_intrusive>",
            "value": 131.57915770116145,
            "unit": "ns/iter",
            "extra": "iterations: 5304127\ncpu: 131.57075594155296 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_intrusive>",
            "value": 3.3866276526093744,
            "unit": "ns/iter",
            "extra": "iterations: 200725940\ncpu: 3.3864665523549133 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_intrusive>",
            "value": 100.09136631948498,
            "unit": "ns/iter",
            "extra": "iterations: 7313231\ncpu: 100.08680773245113 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_intrusive>",
            "value": 3.35196818065429,
            "unit": "ns/iter",
            "extra": "iterations: 208956403\ncpu: 3.3518678200064618 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_intrusive>",
            "value": 3.3438798081801004,
            "unit": "ns/iter",
            "extra": "iterations: 208467698\ncpu: 3.343732610315479 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_intrusive>",
            "value": 3.313912642109479,
            "unit": "ns/iter",
            "extra": "iterations: 209383536\ncpu: 3.3137300489566743 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_intrusive>",
            "value": 3.3192615391505833,
            "unit": "ns/iter",
            "extra": "iterations: 209700054\ncpu: 3.319202874406502 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_intrusive>",
            "value": 3.298649828158501,
            "unit": "ns/iter",
            "extra": "iterations: 212978964\ncpu: 3.2985417846243297 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_intrusive>",
            "value": 3.8774430769320634,
            "unit": "ns/iter",
            "extra": "iterations: 180050204\ncpu: 3.877333024293604 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_intrusive>",
            "value": 3.2969199003592085,
            "unit": "ns/iter",
            "extra": "iterations: 211258750\ncpu: 3.296606090871978 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_intrusive>",
            "value": 3.3062293764149007,
            "unit": "ns/iter",
            "extra": "iterations: 211621487\ncpu: 3.3060671197344043 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_intrusive>",
            "value": 3.310740837369542,
            "unit": "ns/iter",
            "extra": "iterations: 212056325\ncpu: 3.3105386929628224 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_intrusive>",
            "value": 3.2867982661717496,
            "unit": "ns/iter",
            "extra": "iterations: 212394973\ncpu: 3.2866036570460655 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_intrusive>",
            "value": 8.899999954792293e-10,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 5.6999999742402e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_intrusive>",
            "value": 47.49338898108335,
            "unit": "ns/iter",
            "extra": "iterations: 14772685\ncpu: 47.49163621914371 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_intrusive>",
            "value": 34.98597267367106,
            "unit": "ns/iter",
            "extra": "iterations: 19944428\ncpu: 34.984673513825435 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_intrusive>",
            "value": 34.889569260261624,
            "unit": "ns/iter",
            "extra": "iterations: 20002148\ncpu: 34.889248844674036 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_intrusive>",
            "value": 3.4651760021645686,
            "unit": "ns/iter",
            "extra": "iterations: 202076009\ncpu: 3.464913393058958 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_intrusive>",
            "value": 3.4405097226531454,
            "unit": "ns/iter",
            "extra": "iterations: 203281293\ncpu: 3.440285875198543 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_intrusive>",
            "value": 3.3529845587752463,
            "unit": "ns/iter",
            "extra": "iterations: 207509383\ncpu: 3.3526198186421325 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_intrusive>",
            "value": 4.225765900793047,
            "unit": "ns/iter",
            "extra": "iterations: 166021861\ncpu: 4.225477119546286 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert/threads:4",
            "value": 78.78859352889468,
            "unit": "ns/iter",
            "extra": "iterations: 8740144\ncpu: 78.60238309574764 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/emplace/threads:4",
            "value": 79.63999522143389,
            "unit": "ns/iter",
            "extra": "iterations: 8730648\ncpu: 79.53255405555237 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update_insert/threads:4",
            "value": 80.43438545555954,
            "unit": "ns/iter",
            "extra": "iterations: 8632964\ncpu: 80.20373153415213 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update/threads:4",
            "value": 80.15071072898597,
            "unit": "ns/iter",
            "extra": "iterations: 8661248\ncpu: 80.11659301292377 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/find/threads:4",
            "value": 50.49012910191342,
            "unit": "ns/iter",
            "extra": "iterations: 14034184\ncpu: 50.42241494054803 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert_overflow/threads:4",
            "value": 65.28668779406463,
            "unit": "ns/iter",
            "extra": "iterations: 10685652\ncpu: 65.08950974634023 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request/threads:4",
            "value": 99.25176027869392,
            "unit": "ns/iter",
            "extra": "iterations: 7050020\ncpu: 99.11583002601414 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request_hot/threads:4",
            "value": 99.46004310300543,
            "unit": "ns/iter",
            "extra": "iterations: 7065864\ncpu: 99.30948968165818 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/many_shards/threads:4",
            "value": 75.75154356842893,
            "unit": "ns/iter",
            "extra": "iterations: 8728476\ncpu: 75.43098589032036 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert/threads:4",
            "value": 109.8074711725216,
            "unit": "ns/iter",
            "extra": "iterations: 5506900\ncpu: 109.43120957344425 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/emplace/threads:4",
            "value": 110.10616044532772,
            "unit": "ns/iter",
            "extra": "iterations: 5412336\ncpu: 109.90571132316988 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update_insert/threads:4",
            "value": 112.16375745443342,
            "unit": "ns/iter",
            "extra": "iterations: 6266072\ncpu: 112.03125817896762 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update/threads:4",
            "value": 109.63378459736984,
            "unit": "ns/iter",
            "extra": "iterations: 5587564\ncpu: 109.23202526181358 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/find/threads:4",
            "value": 53.65695470096642,
            "unit": "ns/iter",
            "extra": "iterations: 13140148\ncpu: 53.61011778558354 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert_overflow/threads:4",
            "value": 138.65255791739202,
            "unit": "ns/iter",
            "extra": "iterations: 4045244\ncpu: 136.02883905148846 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request/threads:4",
            "value": 142.05834924775826,
            "unit": "ns/iter",
            "extra": "iterations: 4274948\ncpu: 140.97472787973084 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request_hot/threads:4",
            "value": 138.6663737992703,
            "unit": "ns/iter",
            "extra": "iterations: 4578864\ncpu: 138.08079580437422 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/many_shards/threads:4",
            "value": 83.39928937403121,
            "unit": "ns/iter",
            "extra": "iterations: 7942012\ncpu: 83.25941310589819 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert/threads:4",
            "value": 516.8042740225727,
            "unit": "ns/iter",
            "extra": "iterations: 1741404\ncpu: 515.9097756752599 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/emplace/threads:4",
            "value": 514.083470598004,
            "unit": "ns/iter",
            "extra": "iterations: 1780016\ncpu: 513.0801745602291 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update_insert/threads:4",
            "value": 510.73788542018224,
            "unit": "ns/iter",
            "extra": "iterations: 1768200\ncpu: 509.25506956226656 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update/threads:4",
            "value": 499.95079606673784,
            "unit": "ns/iter",
            "extra": "iterations: 1643832\ncpu: 499.31063393339497 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/find/threads:4",
            "value": 116.02102543171358,
            "unit": "ns/iter",
            "extra": "iterations: 6169148\ncpu: 115.91495422058284 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert_overflow/threads:4",
            "value": 457.8876670092488,
            "unit": "ns/iter",
            "extra": "iterations: 1790320\ncpu: 457.5007317127663 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request/threads:4",
            "value": 540.0579342781045,
            "unit": "ns/iter",
            "extra": "iterations: 1648644\ncpu: 538.2672614585078 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request_hot/threads:4",
            "value": 520.9870820280086,
            "unit": "ns/iter",
            "extra": "iterations: 1721168\ncpu: 514.4702312615619 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/many_shards/threads:4",
            "value": 478.1775017770323,
            "unit": "ns/iter",
            "extra": "iterations: 1873908\ncpu: 477.2980210341178 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert/threads:4",
            "value": 131.64542562530505,
            "unit": "ns/iter",
            "extra": "iterations: 5071644\ncpu: 131.21851454873388 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/emplace/threads:4",
            "value": 128.9234017109042,
            "unit": "ns/iter",
            "extra": "iterations: 5452080\ncpu: 128.82246096902483 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update_insert/threads:4",
            "value": 127.97486879483857,
            "unit": "ns/iter",
            "extra": "iterations: 5399940\ncpu: 127.85517653899858 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update/threads:4",
            "value": 126.45757421518437,
            "unit": "ns/iter",
            "extra": "iterations: 5536468\ncpu: 126.29982021028597 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/find/threads:4",
            "value": 58.07796325576865,
            "unit": "ns/iter",
            "extra": "iterations: 11882572\ncpu: 57.92563386108664 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert_overflow/threads:4",
            "value": 153.49640665835526,
            "unit": "ns/iter",
            "extra": "iterations: 4129304\ncpu: 153.2705593969345 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request/threads:4",
            "value": 158.68179089770808,
            "unit": "ns/iter",
            "extra": "iterations: 4388168\ncpu: 158.46916275766992 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request_hot/threads:4",
            "value": 159.7330096105037,
            "unit": "ns/iter",
            "extra": "iterations: 4348576\ncpu: 158.95224712641524 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/many_shards/threads:4",
            "value": 112.47135144664577,
            "unit": "ns/iter",
            "extra": "iterations: 5445720\ncpu: 111.42037196183422 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert/threads:4",
            "value": 97.35654188676197,
            "unit": "ns/iter",
            "extra": "iterations: 6910728\ncpu: 97.19138056077433 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/emplace/threads:4",
            "value": 96.4397881531458,
            "unit": "ns/iter",
            "extra": "iterations: 7109664\ncpu: 96.28409936109493 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update_insert/threads:4",
            "value": 104.15812225888324,
            "unit": "ns/iter",
            "extra": "iterations: 6776440\ncpu: 103.3944199314095 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update/threads:4",
            "value": 100.79922034394427,
            "unit": "ns/iter",
            "extra": "iterations: 6750156\ncpu: 100.6031748007008 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/find/threads:4",
            "value": 52.43519317499584,
            "unit": "ns/iter",
            "extra": "iterations: 13197360\ncpu: 52.364999211963614 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert_overflow/threads:4",
            "value": 135.79862872301533,
            "unit": "ns/iter",
            "extra": "iterations: 3807400\ncpu: 135.2652387456009 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request/threads:4",
            "value": 131.2477024527979,
            "unit": "ns/iter",
            "extra": "iterations: 4481736\ncpu: 131.08559384131493 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request_hot/threads:4",
            "value": 127.40607680041252,
            "unit": "ns/iter",
            "extra": "iterations: 4600184\ncpu: 127.27564158303241 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/many_shards/threads:4",
            "value": 78.78906951880364,
            "unit": "ns/iter",
            "extra": "iterations: 8410444\ncpu: 78.64068722174476 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert/threads:4",
            "value": 208.95865046375326,
            "unit": "ns/iter",
            "extra": "iterations: 3668312\ncpu: 207.9302960053561 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/emplace/threads:4",
            "value": 205.11110585541897,
            "unit": "ns/iter",
            "extra": "iterations: 3784256\ncpu: 204.86183915675912 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update_insert/threads:4",
            "value": 139.0889967888476,
            "unit": "ns/iter",
            "extra": "iterations: 4128104\ncpu: 138.96681721196944 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update/threads:4",
            "value": 132.65380599791538,
            "unit": "ns/iter",
            "extra": "iterations: 4375252\ncpu: 132.5283521954848 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/find/threads:4",
            "value": 76.943533558347,
            "unit": "ns/iter",
            "extra": "iterations: 9023448\ncpu: 76.73599338080075 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert_overflow/threads:4",
            "value": 240.26373562111917,
            "unit": "ns/iter",
            "extra": "iterations: 3095164\ncpu: 240.11075051273488 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request/threads:4",
            "value": 248.419912959605,
            "unit": "ns/iter",
            "extra": "iterations: 3159912\ncpu: 248.0242231429231 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request_hot/threads:4",
            "value": 248.9683774400971,
            "unit": "ns/iter",
            "extra": "iterations: 2842496\ncpu: 248.59136301335215 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/many_shards/threads:4",
            "value": 178.24640755660303,
            "unit": "ns/iter",
            "extra": "iterations: 3857820\ncpu: 178.03071164543692 ns\nthreads: 4"
          },
          {
            "name": "insert<dm_cache_std>",
            "value": 1.4812446649814341,
            "unit": "ns/iter",
            "extra": "iterations: 469363410\ncpu: 1.4811359879970192 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_std>",
            "value": 1.546872567883696,
            "unit": "ns/iter",
            "extra": "iterations: 452409500\ncpu: 1.5467772206374977 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_std>",
            "value": 1.5160902350717014,
            "unit": "ns/iter",
            "extra": "iterations: 460500637\ncpu: 1.5159244915441892 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_std>",
            "value": 1.5168372463205644,
            "unit": "ns/iter",
            "extra": "iterations: 459695211\ncpu: 1.5167412653337384 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_std>",
            "value": 8.929999921747367e-10,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 4.3899999990770767e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_std>",
            "value": 1.5188590239912596,
            "unit": "ns/iter",
            "extra": "iterations: 462503362\ncpu: 1.5187050013271048 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_std>",
            "value": 3.5299310646074575,
            "unit": "ns/iter",
            "extra": "iterations: 197862658\ncpu: 3.5296967606692093 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_std>",
            "value": 3.544136509629563,
            "unit": "ns/iter",
            "extra": "iterations: 189431325\ncpu: 3.543744541722441 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_std>",
            "value": 3.567671671465976,
            "unit": "ns/iter",
            "extra": "iterations: 195124972\ncpu: 3.567435048754295 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_std>",
            "value": 3.5649324332601395,
            "unit": "ns/iter",
            "extra": "iterations: 196270236\ncpu: 3.564853929252936 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_std>",
            "value": 1.1010000093847338e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 4.950000001002764e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_std>",
            "value": 20.513669563161613,
            "unit": "ns/iter",
            "extra": "iterations: 33903278\ncpu: 20.51319506627056 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_std>",
            "value": 34.83843742992307,
            "unit": "ns/iter",
            "extra": "iterations: 19781692\ncpu: 34.83729253291382 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_std>",
            "value": 34.92506074967383,
            "unit": "ns/iter",
            "extra": "iterations: 19746608\ncpu: 34.92157549286436 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_std>",
            "value": 34.88551580897188,
            "unit": "ns/iter",
            "extra": "iterations: 19790156\ncpu: 34.88433901177933 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_std>",
            "value": 34.447664366614724,
            "unit": "ns/iter",
            "extra": "iterations: 20430411\ncpu: 34.44522295709078 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_std>",
            "value": 4.976763404722418,
            "unit": "ns/iter",
            "extra": "iterations: 133830579\ncpu: 4.976535654082469 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_std>",
            "value": 60.24666351821733,
            "unit": "ns/iter",
            "extra": "iterations: 11548542\ncpu: 60.24124430599122 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_std>",
            "value": 5.8952819213122165,
            "unit": "ns/iter",
            "extra": "iterations: 112816728\ncpu: 5.895154865686248 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_std>",
            "value": 5.761152458792055,
            "unit": "ns/iter",
            "extra": "iterations: 130901201\ncpu: 5.76093181146595 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_std>",
            "value": 5.516005264328235,
            "unit": "ns/iter",
            "extra": "iterations: 120826808\ncpu: 5.51561501152957 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_std>",
            "value": 5.4289397892657005,
            "unit": "ns/iter",
            "extra": "iterations: 127917324\ncpu: 5.428743060635003 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_std>",
            "value": 4.950000000706236,
            "unit": "ns/iter",
            "extra": "iterations: 141600242\ncpu: 4.949508560868144 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_std>",
            "value": 20.75724066930729,
            "unit": "ns/iter",
            "extra": "iterations: 34274732\ncpu: 20.756794684784037 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_std>",
            "value": 3.3199961855829314,
            "unit": "ns/iter",
            "extra": "iterations: 210160552\ncpu: 3.3198321633643237 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_std>",
            "value": 3.324744380838671,
            "unit": "ns/iter",
            "extra": "iterations: 210213799\ncpu: 3.3246553571870914 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_std>",
            "value": 3.3552282036849603,
            "unit": "ns/iter",
            "extra": "iterations: 208877784\ncpu: 3.3551163248648788 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_std>",
            "value": 3.332691247358747,
            "unit": "ns/iter",
            "extra": "iterations: 207233411\ncpu: 3.3325329813733484 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_std>",
            "value": 1.0679999888907332e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 5.399999984945225e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_std>",
            "value": 20.59552097939493,
            "unit": "ns/iter",
            "extra": "iterations: 34259186\ncpu: 20.594388056972527 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_std>",
            "value": 36.76264451738381,
            "unit": "ns/iter",
            "extra": "iterations: 19031707\ncpu: 36.76009629614411 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_std>",
            "value": 36.73865630553978,
            "unit": "ns/iter",
            "extra": "iterations: 18937966\ncpu: 36.737993932399995 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_std>",
            "value": 5.6251062649745815,
            "unit": "ns/iter",
            "extra": "iterations: 111377244\ncpu: 5.624768072013003 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_std>",
            "value": 5.547061492232723,
            "unit": "ns/iter",
            "extra": "iterations: 125405692\ncpu: 5.54690005617926 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_std>",
            "value": 4.954621371089356,
            "unit": "ns/iter",
            "extra": "iterations: 133820394\ncpu: 4.954446397759076 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_std>",
            "value": 21.1152254864736,
            "unit": "ns/iter",
            "extra": "iterations: 32922751\ncpu: 21.114690385381383 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert/threads:4",
            "value": 78.69932072431845,
            "unit": "ns/iter",
            "extra": "iterations: 8828816\ncpu: 78.6251492838904 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/emplace/threads:4",
            "value": 79.39793074185656,
            "unit": "ns/iter",
            "extra": "iterations: 8771936\ncpu: 79.22439231202782 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update_insert/threads:4",
            "value": 79.20936490587697,
            "unit": "ns/iter",
            "extra": "iterations: 8604016\ncpu: 78.73713298534079 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update/threads:4",
            "value": 79.16258057887742,
            "unit": "ns/iter",
            "extra": "iterations: 8799452\ncpu: 79.05722901835249 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/find/threads:4",
            "value": 50.54757494741067,
            "unit": "ns/iter",
            "extra": "iterations: 13677724\ncpu: 50.49668585211986 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert_overflow/threads:4",
            "value": 65.80332703534897,
            "unit": "ns/iter",
            "extra": "iterations: 10663728\ncpu: 65.42404485560772 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request/threads:4",
            "value": 99.75124567691635,
            "unit": "ns/iter",
            "extra": "iterations: 7053996\ncpu: 99.54896586275349 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request_hot/threads:4",
            "value": 100.646051166394,
            "unit": "ns/iter",
            "extra": "iterations: 7076520\ncpu: 97.55658727736234 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/many_shards/threads:4",
            "value": 72.97849423239013,
            "unit": "ns/iter",
            "extra": "iterations: 9014140\ncpu: 72.92614037501085 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert/threads:4",
            "value": 102.377450783157,
            "unit": "ns/iter",
            "extra": "iterations: 6561168\ncpu: 102.29192043245959 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/emplace/threads:4",
            "value": 107.03003103179044,
            "unit": "ns/iter",
            "extra": "iterations: 5183072\ncpu: 106.93845001574356 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update_insert/threads:4",
            "value": 120.95583464442215,
            "unit": "ns/iter",
            "extra": "iterations: 5166176\ncpu: 120.87157193250869 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update/threads:4",
            "value": 113.16071503662538,
            "unit": "ns/iter",
            "extra": "iterations: 5421988\ncpu: 112.98792269551316 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/find/threads:4",
            "value": 40.33229097838944,
            "unit": "ns/iter",
            "extra": "iterations: 17241088\ncpu: 40.232134132138306 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert_overflow/threads:4",
            "value": 96.9458365497498,
            "unit": "ns/iter",
            "extra": "iterations: 6762180\ncpu: 96.81212567544787 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request/threads:4",
            "value": 111.55524493008568,
            "unit": "ns/iter",
            "extra": "iterations: 4927692\ncpu: 110.6911517602967 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request_hot/threads:4",
            "value": 101.45412035984448,
            "unit": "ns/iter",
            "extra": "iterations: 5349824\ncpu: 101.29827859757631 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/many_shards/threads:4",
            "value": 110.70924015821488,
            "unit": "ns/iter",
            "extra": "iterations: 4875436\ncpu: 110.57952540039496 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert/threads:4",
            "value": 572.9427186331234,
            "unit": "ns/iter",
            "extra": "iterations: 1842676\ncpu: 571.8416829654265 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/emplace/threads:4",
            "value": 589.2666261120613,
            "unit": "ns/iter",
            "extra": "iterations: 1821412\ncpu: 586.7168444042311 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update_insert/threads:4",
            "value": 546.8459344803083,
            "unit": "ns/iter",
            "extra": "iterations: 1768384\ncpu: 546.5814743856541 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update/threads:4",
            "value": 545.2417899039511,
            "unit": "ns/iter",
            "extra": "iterations: 1669408\ncpu: 544.4261852105659 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/find/threads:4",
            "value": 141.74673718051287,
            "unit": "ns/iter",
            "extra": "iterations: 5371428\ncpu: 141.72722467842814 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert_overflow/threads:4",
            "value": 495.0618779930646,
            "unit": "ns/iter",
            "extra": "iterations: 1859304\ncpu: 493.0570692043905 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request/threads:4",
            "value": 587.3700155068284,
            "unit": "ns/iter",
            "extra": "iterations: 1774704\ncpu: 586.6540893579999 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request_hot/threads:4",
            "value": 578.8956194118429,
            "unit": "ns/iter",
            "extra": "iterations: 1579240\ncpu: 578.160300524303 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/many_shards/threads:4",
            "value": 621.1142981745846,
            "unit": "ns/iter",
            "extra": "iterations: 1664716\ncpu: 618.2595974328353 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert/threads:4",
            "value": 165.96868806449382,
            "unit": "ns/iter",
            "extra": "iterations: 4593488\ncpu: 159.65165708498662 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/emplace/threads:4",
            "value": 169.85173580688937,
            "unit": "ns/iter",
            "extra": "iterations: 4725180\ncpu: 158.46946296225775 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update_insert/threads:4",
            "value": 154.46003837559772,
            "unit": "ns/iter",
            "extra": "iterations: 4631068\ncpu: 154.34601456942553 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update/threads:4",
            "value": 156.59427951910783,
            "unit": "ns/iter",
            "extra": "iterations: 4717960\ncpu: 155.63762749154293 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/find/threads:4",
            "value": 48.561360315383446,
            "unit": "ns/iter",
            "extra": "iterations: 14390604\ncpu: 48.53541811031695 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert_overflow/threads:4",
            "value": 151.72872785725463,
            "unit": "ns/iter",
            "extra": "iterations: 4247668\ncpu: 150.69056079712456 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request/threads:4",
            "value": 153.84060873878846,
            "unit": "ns/iter",
            "extra": "iterations: 4193260\ncpu: 153.78736496186715 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request_hot/threads:4",
            "value": 153.3340486134603,
            "unit": "ns/iter",
            "extra": "iterations: 4428568\ncpu: 153.23258308329002 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/many_shards/threads:4",
            "value": 158.321652918646,
            "unit": "ns/iter",
            "extra": "iterations: 4131988\ncpu: 158.12474915222393 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert/threads:4",
            "value": 89.51408655987711,
            "unit": "ns/iter",
            "extra": "iterations: 7794096\ncpu: 88.88360394842468 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/emplace/threads:4",
            "value": 88.67577036257646,
            "unit": "ns/iter",
            "extra": "iterations: 7768420\ncpu: 88.60583052924527 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update_insert/threads:4",
            "value": 93.77015245342302,
            "unit": "ns/iter",
            "extra": "iterations: 7370120\ncpu: 93.57245431553359 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update/threads:4",
            "value": 92.63753961106529,
            "unit": "ns/iter",
            "extra": "iterations: 7533248\ncpu: 91.45543648636031 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/find/threads:4",
            "value": 38.26899972656594,
            "unit": "ns/iter",
            "extra": "iterations: 18242052\ncpu: 38.19772742671717 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert_overflow/threads:4",
            "value": 102.79134554227056,
            "unit": "ns/iter",
            "extra": "iterations: 5156880\ncpu: 102.6404387924481 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request/threads:4",
            "value": 90.34665060550834,
            "unit": "ns/iter",
            "extra": "iterations: 7573548\ncpu: 89.69109643194979 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request_hot/threads:4",
            "value": 89.80348434818674,
            "unit": "ns/iter",
            "extra": "iterations: 7717828\ncpu: 89.76724746910656 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/many_shards/threads:4",
            "value": 92.92988604408613,
            "unit": "ns/iter",
            "extra": "iterations: 7383908\ncpu: 92.72289782050376 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert/threads:4",
            "value": 212.15844520947417,
            "unit": "ns/iter",
            "extra": "iterations: 3519368\ncpu: 212.0060939918757 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/emplace/threads:4",
            "value": 212.42237995783333,
            "unit": "ns/iter",
            "extra": "iterations: 3615896\ncpu: 211.1190811350772 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update_insert/threads:4",
            "value": 158.84737144630623,
            "unit": "ns/iter",
            "extra": "iterations: 4022288\ncpu: 157.25453075463514 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update/threads:4",
            "value": 175.55299728478815,
            "unit": "ns/iter",
            "extra": "iterations: 4264860\ncpu: 174.55179396275594 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/find/threads:4",
            "value": 70.80605656758446,
            "unit": "ns/iter",
            "extra": "iterations: 9772664\ncpu: 70.38950372181014 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert_overflow/threads:4",
            "value": 215.91892230776048,
            "unit": "ns/iter",
            "extra": "iterations: 3285836\ncpu: 215.8151955849281 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request/threads:4",
            "value": 216.61786928243023,
            "unit": "ns/iter",
            "extra": "iterations: 3400920\ncpu: 215.1306687602179 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request_hot/threads:4",
            "value": 212.42922084715448,
            "unit": "ns/iter",
            "extra": "iterations: 3097056\ncpu: 212.34776284316442 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/many_shards/threads:4",
            "value": 219.69013683497897,
            "unit": "ns/iter",
            "extra": "iterations: 3273432\ncpu: 218.2391603063696 ns\nthreads: 4"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wstux1@gmail.com",
            "name": "wstux",
            "username": "wstux"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "51eeff15019e8731f917e7ba5fbef5f9af228401",
          "message": "* all: #42 Merge remote-tracking branch 'wstux/bugfix/cache' into master;",
          "timestamp": "2026-11-20T22:17:27+03:00",
          "tree_id": "1e7b45c0bf6d7431886e0287aefdceb5797a5b92",
          "url": "https://github.com/wstux/thread_safe_lru_cache/commit/51eeff15019e8731f917e7ba5fbef5f9af228401"
        },
        "date": 1795202507142,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "insert<dm_cache_intrusive>",
            "value": 2.485142446069612,
            "unit": "ns/iter",
            "extra": "iterations: 277099011\ncpu: 2.4847740903701747 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_intrusive>",
            "value": 2.482054932162551,
            "unit": "ns/iter",
            "extra": "iterations: 281486314\ncpu: 2.480689490999551 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_intrusive>",
            "value": 2.484440028745724,
            "unit": "ns/iter",
            "extra": "iterations: 281517808\ncpu: 2.4830417868272128 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_intrusive>",
            "value": 2.4866935985647776,
            "unit": "ns/iter",
            "extra": "iterations: 282599959\ncpu: 2.4854356436760856 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_intrusive>",
            "value": 1.7529999922771821e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.3020000002583743e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_intrusive>",
            "value": 2.4751430374796475,
            "unit": "ns/iter",
            "extra": "iterations: 283103597\ncpu: 2.4740819630066384 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_intrusive>",
            "value": 2.5201403731379663,
            "unit": "ns/iter",
            "extra": "iterations: 277136499\ncpu: 2.5193307684817077 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_intrusive>",
            "value": 2.521435153725412,
            "unit": "ns/iter",
            "extra": "iterations: 277167688\ncpu: 2.5184575158703213 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_intrusive>",
            "value": 2.5851898188878732,
            "unit": "ns/iter",
            "extra": "iterations: 269973687\ncpu: 2.5849215742273453 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_intrusive>",
            "value": 2.579280728234911,
            "unit": "ns/iter",
            "extra": "iterations: 271627790\ncpu: 2.579115472684149 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_intrusive>",
            "value": 2.477035248194746,
            "unit": "ns/iter",
            "extra": "iterations: 282134902\ncpu: 2.476886953887048 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_intrusive>",
            "value": 4.253272557015137,
            "unit": "ns/iter",
            "extra": "iterations: 164262226\ncpu: 4.252738551101817 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_intrusive>",
            "value": 169.54192838206316,
            "unit": "ns/iter",
            "extra": "iterations: 4191576\ncpu: 169.52965590985337 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_intrusive>",
            "value": 169.99243368748267,
            "unit": "ns/iter",
            "extra": "iterations: 4194910\ncpu: 169.981545968805 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_intrusive>",
            "value": 170.9854843513895,
            "unit": "ns/iter",
            "extra": "iterations: 4166469\ncpu: 170.9704176366126 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_intrusive>",
            "value": 171.38349691191283,
            "unit": "ns/iter",
            "extra": "iterations: 4070317\ncpu: 171.37124479493843 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_intrusive>",
            "value": 2.562581283687847,
            "unit": "ns/iter",
            "extra": "iterations: 262596507\ncpu: 2.562385904089729 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_intrusive>",
            "value": 129.95654793659145,
            "unit": "ns/iter",
            "extra": "iterations: 5632805\ncpu: 129.94202071614433 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_intrusive>",
            "value": 3.0630814364526744,
            "unit": "ns/iter",
            "extra": "iterations: 228539485\ncpu: 3.0628606824768188 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_intrusive>",
            "value": 3.067828541512441,
            "unit": "ns/iter",
            "extra": "iterations: 228289591\ncpu: 3.0674766156990545 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_intrusive>",
            "value": 3.204846798287589,
            "unit": "ns/iter",
            "extra": "iterations: 216032964\ncpu: 3.2047086804771125 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_intrusive>",
            "value": 3.2050674743700296,
            "unit": "ns/iter",
            "extra": "iterations: 217581209\ncpu: 3.204679632973265 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_intrusive>",
            "value": 2.4764588863744903,
            "unit": "ns/iter",
            "extra": "iterations: 281889808\ncpu: 2.4763810616380963 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_intrusive>",
            "value": 4.206615466526914,
            "unit": "ns/iter",
            "extra": "iterations: 165408924\ncpu: 4.2059011519838 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_intrusive>",
            "value": 2.4873715671664507,
            "unit": "ns/iter",
            "extra": "iterations: 280140105\ncpu: 2.4871528373276024 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_intrusive>",
            "value": 2.48983737176872,
            "unit": "ns/iter",
            "extra": "iterations: 280217768\ncpu: 2.489502275244729 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_intrusive>",
            "value": 2.518480592742775,
            "unit": "ns/iter",
            "extra": "iterations: 274813588\ncpu: 2.5182525145008574 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_intrusive>",
            "value": 2.512127038172792,
            "unit": "ns/iter",
            "extra": "iterations: 278491537\ncpu: 2.5117041348369553 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_intrusive>",
            "value": 1.9130000055156415e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.531999998860556e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_intrusive>",
            "value": 45.45527901733416,
            "unit": "ns/iter",
            "extra": "iterations: 15352469\ncpu: 45.448344106736165 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_intrusive>",
            "value": 35.018354626344085,
            "unit": "ns/iter",
            "extra": "iterations: 19956549\ncpu: 35.01310497120512 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_intrusive>",
            "value": 35.02793650927249,
            "unit": "ns/iter",
            "extra": "iterations: 19966632\ncpu: 35.022070622626735 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_intrusive>",
            "value": 3.2071608514462078,
            "unit": "ns/iter",
            "extra": "iterations: 217977319\ncpu: 3.2069967242784765 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_intrusive>",
            "value": 3.187154451287466,
            "unit": "ns/iter",
            "extra": "iterations: 220288044\ncpu: 3.1867589554701476 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_intrusive>",
            "value": 2.596014168827377,
            "unit": "ns/iter",
            "extra": "iterations: 262914347\ncpu: 2.5959513765142654 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_intrusive>",
            "value": 4.549594213849941,
            "unit": "ns/iter",
            "extra": "iterations: 152904430\ncpu: 4.548771530033521 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert/threads:4",
            "value": 70.11937229616807,
            "unit": "ns/iter",
            "extra": "iterations: 9662200\ncpu: 69.8719861936205 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/emplace/threads:4",
            "value": 69.03467704914402,
            "unit": "ns/iter",
            "extra": "iterations: 9933400\ncpu: 68.94080516238147 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update_insert/threads:4",
            "value": 67.92059573959571,
            "unit": "ns/iter",
            "extra": "iterations: 10185524\ncpu: 67.84535670427952 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update/threads:4",
            "value": 68.36275540194936,
            "unit": "ns/iter",
            "extra": "iterations: 10302584\ncpu: 67.68213285133126 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/find/threads:4",
            "value": 44.47540061509486,
            "unit": "ns/iter",
            "extra": "iterations: 15858364\ncpu: 44.42170661488159 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert_overflow/threads:4",
            "value": 57.62405174018137,
            "unit": "ns/iter",
            "extra": "iterations: 12017276\ncpu: 57.603348046595606 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request/threads:4",
            "value": 86.75240084545142,
            "unit": "ns/iter",
            "extra": "iterations: 8084652\ncpu: 85.94122567056688 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request_hot/threads:4",
            "value": 85.2590108092308,
            "unit": "ns/iter",
            "extra": "iterations: 8222236\ncpu: 85.08537957314775 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/many_shards/threads:4",
            "value": 60.481326250990364,
            "unit": "ns/iter",
            "extra": "iterations: 10655600\ncpu: 60.41061901723037 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert/threads:4",
            "value": 167.13175750000175,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 167.01296625 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/emplace/threads:4",
            "value": 129.49898690033038,
            "unit": "ns/iter",
            "extra": "iterations: 5649000\ncpu: 128.31161710037176 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update_insert/threads:4",
            "value": 135.0727617499956,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 134.99041075000005 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update/threads:4",
            "value": 131.56926250000822,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 131.51016474999997 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/find/threads:4",
            "value": 46.011805094087926,
            "unit": "ns/iter",
            "extra": "iterations: 15259260\ncpu: 45.98578024098153 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert_overflow/threads:4",
            "value": 175.21536680387428,
            "unit": "ns/iter",
            "extra": "iterations: 5039096\ncpu: 173.75171340256276 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request/threads:4",
            "value": 157.33203354868468,
            "unit": "ns/iter",
            "extra": "iterations: 5111616\ncpu: 157.12055972123105 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request_hot/threads:4",
            "value": 147.6808970779957,
            "unit": "ns/iter",
            "extra": "iterations: 5205612\ncpu: 146.78546441801646 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/many_shards/threads:4",
            "value": 82.72867740190182,
            "unit": "ns/iter",
            "extra": "iterations: 8381624\ncpu: 82.59710492859142 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert/threads:4",
            "value": 605.8129292851781,
            "unit": "ns/iter",
            "extra": "iterations: 1884528\ncpu: 602.2230574446228 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/emplace/threads:4",
            "value": 613.4529245907318,
            "unit": "ns/iter",
            "extra": "iterations: 1788216\ncpu: 612.8903281259085 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update_insert/threads:4",
            "value": 632.1673635965063,
            "unit": "ns/iter",
            "extra": "iterations: 1762492\ncpu: 630.3998713185651 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update/threads:4",
            "value": 619.1095664538612,
            "unit": "ns/iter",
            "extra": "iterations: 1590788\ncpu: 617.8844880650346 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/find/threads:4",
            "value": 106.12239362891684,
            "unit": "ns/iter",
            "extra": "iterations: 6653312\ncpu: 105.90452243935049 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert_overflow/threads:4",
            "value": 532.5670157411395,
            "unit": "ns/iter",
            "extra": "iterations: 1858056\ncpu: 532.2616132129498 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request/threads:4",
            "value": 621.0873562570729,
            "unit": "ns/iter",
            "extra": "iterations: 1738868\ncpu: 616.617144027034 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request_hot/threads:4",
            "value": 603.4358017484491,
            "unit": "ns/iter",
            "extra": "iterations: 1600028\ncpu: 602.8183194294105 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/many_shards/threads:4",
            "value": 511.81891565003815,
            "unit": "ns/iter",
            "extra": "iterations: 1947932\ncpu: 511.1642747282761 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert/threads:4",
            "value": 148.00888100000975,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 147.35413000000014 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/emplace/threads:4",
            "value": 147.09159800001714,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 146.07053024999985 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update_insert/threads:4",
            "value": 152.23595161892678,
            "unit": "ns/iter",
            "extra": "iterations: 6282788\ncpu: 152.16774368321833 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update/threads:4",
            "value": 144.8175342499951,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 144.70389000000003 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/find/threads:4",
            "value": 50.190108511678055,
            "unit": "ns/iter",
            "extra": "iterations: 13967160\ncpu: 50.13332237906629 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert_overflow/threads:4",
            "value": 154.60899722786206,
            "unit": "ns/iter",
            "extra": "iterations: 5265244\ncpu: 153.66687108897523 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request/threads:4",
            "value": 159.0442846046635,
            "unit": "ns/iter",
            "extra": "iterations: 5154884\ncpu: 158.80568738307207 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request_hot/threads:4",
            "value": 155.23450430230722,
            "unit": "ns/iter",
            "extra": "iterations: 5150720\ncpu: 153.6313257175694 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/many_shards/threads:4",
            "value": 109.20528963364514,
            "unit": "ns/iter",
            "extra": "iterations: 6669460\ncpu: 109.07215471717358 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert/threads:4",
            "value": 95.1061114832006,
            "unit": "ns/iter",
            "extra": "iterations: 7262296\ncpu: 94.99126405753768 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/emplace/threads:4",
            "value": 100.42862950004923,
            "unit": "ns/iter",
            "extra": "iterations: 6920044\ncpu: 99.2777173093121 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update_insert/threads:4",
            "value": 102.36389440140049,
            "unit": "ns/iter",
            "extra": "iterations: 7040548\ncpu: 101.752020013215 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update/threads:4",
            "value": 103.28109770192552,
            "unit": "ns/iter",
            "extra": "iterations: 6593156\ncpu: 103.16178443222019 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/find/threads:4",
            "value": 45.836937627664035,
            "unit": "ns/iter",
            "extra": "iterations: 15348728\ncpu: 45.68658360484336 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert_overflow/threads:4",
            "value": 126.5272361380804,
            "unit": "ns/iter",
            "extra": "iterations: 5148564\ncpu: 126.2969128867777 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request/threads:4",
            "value": 121.0161582899825,
            "unit": "ns/iter",
            "extra": "iterations: 5276796\ncpu: 120.77810265926504 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request_hot/threads:4",
            "value": 128.88313489615635,
            "unit": "ns/iter",
            "extra": "iterations: 5218444\ncpu: 128.69953284925552 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/many_shards/threads:4",
            "value": 72.5460762349262,
            "unit": "ns/iter",
            "extra": "iterations: 8541388\ncpu: 72.14147524969003 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert/threads:4",
            "value": 269.3307144985831,
            "unit": "ns/iter",
            "extra": "iterations: 3758160\ncpu: 268.9760196479129 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/emplace/threads:4",
            "value": 262.3248482240962,
            "unit": "ns/iter",
            "extra": "iterations: 3704804\ncpu: 262.19565083604977 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update_insert/threads:4",
            "value": 180.5637180762901,
            "unit": "ns/iter",
            "extra": "iterations: 4964180\ncpu: 180.0318677002041 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update/threads:4",
            "value": 184.32848799428572,
            "unit": "ns/iter",
            "extra": "iterations: 4924320\ncpu: 180.89949576794336 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/find/threads:4",
            "value": 64.30088847833929,
            "unit": "ns/iter",
            "extra": "iterations: 10875448\ncpu: 64.28797756193583 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert_overflow/threads:4",
            "value": 265.81059783100284,
            "unit": "ns/iter",
            "extra": "iterations: 3755844\ncpu: 263.4992598201633 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request/threads:4",
            "value": 269.8921604116932,
            "unit": "ns/iter",
            "extra": "iterations: 3685168\ncpu: 268.48442106302923 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request_hot/threads:4",
            "value": 287.34747436109353,
            "unit": "ns/iter",
            "extra": "iterations: 3105432\ncpu: 286.9606985437135 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/many_shards/threads:4",
            "value": 213.38342098965478,
            "unit": "ns/iter",
            "extra": "iterations: 4362492\ncpu: 211.24108605815192 ns\nthreads: 4"
          },
          {
            "name": "insert<dm_cache_std>",
            "value": 1.5307817152783838,
            "unit": "ns/iter",
            "extra": "iterations: 450989049\ncpu: 1.5306090725054393 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_std>",
            "value": 1.522715646203544,
            "unit": "ns/iter",
            "extra": "iterations: 453479175\ncpu: 1.522694264846892 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_std>",
            "value": 1.440462531623765,
            "unit": "ns/iter",
            "extra": "iterations: 487031953\ncpu: 1.440331961135207 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_std>",
            "value": 1.4233758336556168,
            "unit": "ns/iter",
            "extra": "iterations: 491894859\ncpu: 1.4233349651657978 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_std>",
            "value": 2.864000009594747e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.8129999999061397e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_std>",
            "value": 1.3526420243403183,
            "unit": "ns/iter",
            "extra": "iterations: 517816804\ncpu: 1.3525092746893541 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_std>",
            "value": 2.524795823409647,
            "unit": "ns/iter",
            "extra": "iterations: 274538574\ncpu: 2.5247304701160136 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_std>",
            "value": 2.520970856251625,
            "unit": "ns/iter",
            "extra": "iterations: 274285343\ncpu: 2.5206721235556504 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_std>",
            "value": 2.6198641390446515,
            "unit": "ns/iter",
            "extra": "iterations: 264469655\ncpu: 2.6197598888991624 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_std>",
            "value": 2.6059708403953312,
            "unit": "ns/iter",
            "extra": "iterations: 269331567\ncpu: 2.6056345003183354 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_std>",
            "value": 3.185000025496265e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 2.2629999989476345e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_std>",
            "value": 21.68034032118458,
            "unit": "ns/iter",
            "extra": "iterations: 32154096\ncpu: 21.679647936611218 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_std>",
            "value": 38.131892177018514,
            "unit": "ns/iter",
            "extra": "iterations: 17729467\ncpu: 38.12597135604809 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_std>",
            "value": 38.79636882446146,
            "unit": "ns/iter",
            "extra": "iterations: 17557675\ncpu: 38.79452279416268 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_std>",
            "value": 39.93239916449314,
            "unit": "ns/iter",
            "extra": "iterations: 13815347\ncpu: 39.928529699615986 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_std>",
            "value": 38.48326614553432,
            "unit": "ns/iter",
            "extra": "iterations: 18178896\ncpu: 38.48123351385033 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_std>",
            "value": 3.7275195795508433,
            "unit": "ns/iter",
            "extra": "iterations: 187089072\ncpu: 3.7272438980294833 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_std>",
            "value": 74.81217280792723,
            "unit": "ns/iter",
            "extra": "iterations: 8652219\ncpu: 74.8056468519809 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_std>",
            "value": 4.834928949647596,
            "unit": "ns/iter",
            "extra": "iterations: 134499544\ncpu: 4.834322337925547 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_std>",
            "value": 5.239092337463562,
            "unit": "ns/iter",
            "extra": "iterations: 125715225\ncpu: 5.2388430120536364 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_std>",
            "value": 5.321555678809003,
            "unit": "ns/iter",
            "extra": "iterations: 128676073\ncpu: 5.320995061762588 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_std>",
            "value": 5.325246598247962,
            "unit": "ns/iter",
            "extra": "iterations: 131963022\ncpu: 5.324897333739461 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_std>",
            "value": 3.699306799977162,
            "unit": "ns/iter",
            "extra": "iterations: 189491771\ncpu: 3.698876527994461 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_std>",
            "value": 21.74784298156818,
            "unit": "ns/iter",
            "extra": "iterations: 32166160\ncpu: 21.745966537503907 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_std>",
            "value": 2.4945651532241224,
            "unit": "ns/iter",
            "extra": "iterations: 280009182\ncpu: 2.494253842004372 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_std>",
            "value": 2.4878685890562227,
            "unit": "ns/iter",
            "extra": "iterations: 279917935\ncpu: 2.4877480180039164 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_std>",
            "value": 2.5859732109547826,
            "unit": "ns/iter",
            "extra": "iterations: 272409187\ncpu: 2.5858135834457054 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_std>",
            "value": 2.6452162314753953,
            "unit": "ns/iter",
            "extra": "iterations: 274151392\ncpu: 2.6450829328636005 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_std>",
            "value": 2.474000041274849e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.481999998276251e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_std>",
            "value": 21.53166584999306,
            "unit": "ns/iter",
            "extra": "iterations: 31528271\ncpu: 21.52883800700648 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_std>",
            "value": 35.81998349656015,
            "unit": "ns/iter",
            "extra": "iterations: 19465032\ncpu: 35.81769523933995 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_std>",
            "value": 35.859661805285285,
            "unit": "ns/iter",
            "extra": "iterations: 19472924\ncpu: 35.85386909536535 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_std>",
            "value": 5.289010672741319,
            "unit": "ns/iter",
            "extra": "iterations: 121530069\ncpu: 5.288886942045614 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_std>",
            "value": 5.206275252048617,
            "unit": "ns/iter",
            "extra": "iterations: 134515075\ncpu: 5.20560041318788 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_std>",
            "value": 3.715100569876375,
            "unit": "ns/iter",
            "extra": "iterations: 187726690\ncpu: 3.7148342784928516 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_std>",
            "value": 21.54075634194946,
            "unit": "ns/iter",
            "extra": "iterations: 32395400\ncpu: 21.53885011452242 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert/threads:4",
            "value": 71.90001526553031,
            "unit": "ns/iter",
            "extra": "iterations: 9498520\ncpu: 71.76027002101381 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/emplace/threads:4",
            "value": 70.90206063809231,
            "unit": "ns/iter",
            "extra": "iterations: 9432224\ncpu: 70.43356943176923 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update_insert/threads:4",
            "value": 74.32078898981285,
            "unit": "ns/iter",
            "extra": "iterations: 9589528\ncpu: 73.44595052019245 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update/threads:4",
            "value": 69.78385441472017,
            "unit": "ns/iter",
            "extra": "iterations: 9910672\ncpu: 69.75728386531206 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/find/threads:4",
            "value": 44.4605197081623,
            "unit": "ns/iter",
            "extra": "iterations: 15850896\ncpu: 44.35563383924795 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert_overflow/threads:4",
            "value": 58.25510041498515,
            "unit": "ns/iter",
            "extra": "iterations: 12100584\ncpu: 57.78933322556994 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request/threads:4",
            "value": 86.43071189261093,
            "unit": "ns/iter",
            "extra": "iterations: 8111392\ncpu: 86.30184104035409 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request_hot/threads:4",
            "value": 86.73419149493206,
            "unit": "ns/iter",
            "extra": "iterations: 8071984\ncpu: 86.59484396896721 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/many_shards/threads:4",
            "value": 66.87574221958587,
            "unit": "ns/iter",
            "extra": "iterations: 9714780\ncpu: 66.85758257006339 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert/threads:4",
            "value": 272.7446318759404,
            "unit": "ns/iter",
            "extra": "iterations: 3436396\ncpu: 269.48523074756235 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/emplace/threads:4",
            "value": 262.3680560068109,
            "unit": "ns/iter",
            "extra": "iterations: 4370468\ncpu: 261.1786625597075 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update_insert/threads:4",
            "value": 267.2620713613617,
            "unit": "ns/iter",
            "extra": "iterations: 4833092\ncpu: 265.67446947006175 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update/threads:4",
            "value": 260.00754504667526,
            "unit": "ns/iter",
            "extra": "iterations: 3872872\ncpu: 258.44630031666424 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/find/threads:4",
            "value": 37.613711596921306,
            "unit": "ns/iter",
            "extra": "iterations: 18045688\ncpu: 37.56734888689198 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert_overflow/threads:4",
            "value": 204.93486241069186,
            "unit": "ns/iter",
            "extra": "iterations: 4555296\ncpu: 203.73636071069825 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request/threads:4",
            "value": 204.80742457940104,
            "unit": "ns/iter",
            "extra": "iterations: 4782116\ncpu: 204.53256152715667 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request_hot/threads:4",
            "value": 204.95817674407414,
            "unit": "ns/iter",
            "extra": "iterations: 4955640\ncpu: 204.420472834992 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/many_shards/threads:4",
            "value": 189.87091893004893,
            "unit": "ns/iter",
            "extra": "iterations: 4972348\ncpu: 189.70027359307912 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert/threads:4",
            "value": 785.6999716086102,
            "unit": "ns/iter",
            "extra": "iterations: 1866760\ncpu: 784.1377306134693 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/emplace/threads:4",
            "value": 740.6939510339733,
            "unit": "ns/iter",
            "extra": "iterations: 1563044\ncpu: 738.491583090431 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update_insert/threads:4",
            "value": 760.6902384502309,
            "unit": "ns/iter",
            "extra": "iterations: 1698132\ncpu: 756.8484411105853 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update/threads:4",
            "value": 757.4786674755145,
            "unit": "ns/iter",
            "extra": "iterations: 1401100\ncpu: 755.8196773963315 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/find/threads:4",
            "value": 165.21221831381845,
            "unit": "ns/iter",
            "extra": "iterations: 4530396\ncpu: 165.1659740119848 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert_overflow/threads:4",
            "value": 744.0094368738517,
            "unit": "ns/iter",
            "extra": "iterations: 1516392\ncpu: 739.511737070626 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request/threads:4",
            "value": 767.4684804069702,
            "unit": "ns/iter",
            "extra": "iterations: 1497164\ncpu: 764.195642561536 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request_hot/threads:4",
            "value": 708.9997672402955,
            "unit": "ns/iter",
            "extra": "iterations: 1495104\ncpu: 704.9544934666756 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/many_shards/threads:4",
            "value": 712.442477627136,
            "unit": "ns/iter",
            "extra": "iterations: 1741392\ncpu: 641.0182348374171 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert/threads:4",
            "value": 340.6376058813495,
            "unit": "ns/iter",
            "extra": "iterations: 5066048\ncpu: 338.8615700048637 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/emplace/threads:4",
            "value": 337.0727863623993,
            "unit": "ns/iter",
            "extra": "iterations: 4224088\ncpu: 336.7348859682847 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update_insert/threads:4",
            "value": 410.16666692197305,
            "unit": "ns/iter",
            "extra": "iterations: 3916740\ncpu: 408.82291650709556 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update/threads:4",
            "value": 353.22363829726675,
            "unit": "ns/iter",
            "extra": "iterations: 3573100\ncpu: 352.86010802944264 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/find/threads:4",
            "value": 44.12743774596376,
            "unit": "ns/iter",
            "extra": "iterations: 15636576\ncpu: 44.06842399512533 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert_overflow/threads:4",
            "value": 261.54600743322493,
            "unit": "ns/iter",
            "extra": "iterations: 4731192\ncpu: 261.1590411042294 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request/threads:4",
            "value": 238.65696721455134,
            "unit": "ns/iter",
            "extra": "iterations: 4492728\ncpu: 238.0590362915361 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request_hot/threads:4",
            "value": 279.10160536966123,
            "unit": "ns/iter",
            "extra": "iterations: 4296456\ncpu: 277.1098398307818 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/many_shards/threads:4",
            "value": 294.50611260564347,
            "unit": "ns/iter",
            "extra": "iterations: 4002712\ncpu: 294.2119085759856 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert/threads:4",
            "value": 91.27259018616786,
            "unit": "ns/iter",
            "extra": "iterations: 8214452\ncpu: 91.13485476572276 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/emplace/threads:4",
            "value": 107.27550370233307,
            "unit": "ns/iter",
            "extra": "iterations: 6927504\ncpu: 106.13090948774634 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update_insert/threads:4",
            "value": 121.35015207876648,
            "unit": "ns/iter",
            "extra": "iterations: 7171284\ncpu: 121.19854450053873 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update/threads:4",
            "value": 75.43523603170132,
            "unit": "ns/iter",
            "extra": "iterations: 8314052\ncpu: 75.39601869220918 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/find/threads:4",
            "value": 33.75408519833739,
            "unit": "ns/iter",
            "extra": "iterations: 20780876\ncpu: 33.704537335192185 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert_overflow/threads:4",
            "value": 95.64825596064703,
            "unit": "ns/iter",
            "extra": "iterations: 8164036\ncpu: 95.51418673313063 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request/threads:4",
            "value": 73.44180124307637,
            "unit": "ns/iter",
            "extra": "iterations: 8890860\ncpu: 73.35145981378626 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request_hot/threads:4",
            "value": 68.5639621300227,
            "unit": "ns/iter",
            "extra": "iterations: 9954484\ncpu: 67.78094826411919 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/many_shards/threads:4",
            "value": 81.00254227354208,
            "unit": "ns/iter",
            "extra": "iterations: 8154512\ncpu: 80.89959264269902 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert/threads:4",
            "value": 374.9570651184723,
            "unit": "ns/iter",
            "extra": "iterations: 3515440\ncpu: 374.18620258061526 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/emplace/threads:4",
            "value": 384.4580825410228,
            "unit": "ns/iter",
            "extra": "iterations: 3964768\ncpu: 382.24808387275164 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update_insert/threads:4",
            "value": 331.01025957774885,
            "unit": "ns/iter",
            "extra": "iterations: 4037788\ncpu: 330.3380880818903 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update/threads:4",
            "value": 390.277208616428,
            "unit": "ns/iter",
            "extra": "iterations: 4364316\ncpu: 388.1712806772008 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/find/threads:4",
            "value": 66.67446807197753,
            "unit": "ns/iter",
            "extra": "iterations: 11018784\ncpu: 66.5898597340688 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert_overflow/threads:4",
            "value": 330.26807249100665,
            "unit": "ns/iter",
            "extra": "iterations: 3342760\ncpu: 327.78142403283476 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request/threads:4",
            "value": 357.70420348002216,
            "unit": "ns/iter",
            "extra": "iterations: 3790288\ncpu: 356.812535353514 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request_hot/threads:4",
            "value": 344.02793890297124,
            "unit": "ns/iter",
            "extra": "iterations: 3130760\ncpu: 341.0627441899083 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/many_shards/threads:4",
            "value": 309.14394899119327,
            "unit": "ns/iter",
            "extra": "iterations: 3580560\ncpu: 308.58050472551776 ns\nthreads: 4"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wstux1@gmail.com",
            "name": "wstux",
            "username": "wstux"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "6ac7e356f10679826038f20acc47d4ba78b8dfbc",
          "message": "* all: #43 Merge remote-tracking branch 'wstux/feature/cache_optimization' into master;",
          "timestamp": "2026-11-21T21:48:53+03:00",
          "tree_id": "525c3168358d795a556000655e7b4bd5190bad68",
          "url": "https://github.com/wstux/thread_safe_lru_cache/commit/6ac7e356f10679826038f20acc47d4ba78b8dfbc"
        },
        "date": 1795287152730,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "insert<dm_cache_intrusive>",
            "value": 1.0259054337150753,
            "unit": "ns/iter",
            "extra": "iterations: 689708854\ncpu: 1.0257944709522318 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_intrusive>",
            "value": 1.0002519588815995,
            "unit": "ns/iter",
            "extra": "iterations: 698677494\ncpu: 0.9998393550658726 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_intrusive>",
            "value": 1.0062715390318089,
            "unit": "ns/iter",
            "extra": "iterations: 697834292\ncpu: 1.0058228078593765 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_intrusive>",
            "value": 1.0039750237737057,
            "unit": "ns/iter",
            "extra": "iterations: 696926901\ncpu: 1.0039208516647569 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_intrusive>",
            "value": 1.2520000041149616e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.0520000000013852e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_intrusive>",
            "value": 1.910015243878131,
            "unit": "ns/iter",
            "extra": "iterations: 364837606\ncpu: 1.9099772077772041 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_intrusive>",
            "value": 1.9565433002461745,
            "unit": "ns/iter",
            "extra": "iterations: 347128523\ncpu: 1.9563839558064788 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_intrusive>",
            "value": 1.956609787540178,
            "unit": "ns/iter",
            "extra": "iterations: 354300155\ncpu: 1.9565371542103898 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_intrusive>",
            "value": 2.001283073699062,
            "unit": "ns/iter",
            "extra": "iterations: 346976951\ncpu: 2.000926482289597 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_intrusive>",
            "value": 1.9919055099210183,
            "unit": "ns/iter",
            "extra": "iterations: 350977884\ncpu: 1.9917766129104595 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_intrusive>",
            "value": 1.9225844199797106,
            "unit": "ns/iter",
            "extra": "iterations: 363309078\ncpu: 1.9223499006540083 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_intrusive>",
            "value": 3.2711291498983592,
            "unit": "ns/iter",
            "extra": "iterations: 214652519\ncpu: 3.270985536396156 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_intrusive>",
            "value": 142.4140574836068,
            "unit": "ns/iter",
            "extra": "iterations: 4901119\ncpu: 142.3994318440339 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_intrusive>",
            "value": 141.8972307292934,
            "unit": "ns/iter",
            "extra": "iterations: 4900171\ncpu: 141.88916305165688 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_intrusive>",
            "value": 141.19870613375616,
            "unit": "ns/iter",
            "extra": "iterations: 4983359\ncpu: 141.18285297125902 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_intrusive>",
            "value": 142.47349877541137,
            "unit": "ns/iter",
            "extra": "iterations: 4900849\ncpu: 142.46119294840526 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_intrusive>",
            "value": 1.971110774467283,
            "unit": "ns/iter",
            "extra": "iterations: 338214328\ncpu: 1.9709114748089558 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_intrusive>",
            "value": 110.21195923300604,
            "unit": "ns/iter",
            "extra": "iterations: 6472589\ncpu: 110.20626104948111 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_intrusive>",
            "value": 2.3664774032422975,
            "unit": "ns/iter",
            "extra": "iterations: 294723964\ncpu: 2.366297003931451 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_intrusive>",
            "value": 2.365459147593129,
            "unit": "ns/iter",
            "extra": "iterations: 294155740\ncpu: 2.3652548850483077 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_intrusive>",
            "value": 2.497086619458542,
            "unit": "ns/iter",
            "extra": "iterations: 278603323\ncpu: 2.4969058714349854 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_intrusive>",
            "value": 2.491978050254172,
            "unit": "ns/iter",
            "extra": "iterations: 280949404\ncpu: 2.491857288296644 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_intrusive>",
            "value": 1.923042645191987,
            "unit": "ns/iter",
            "extra": "iterations: 362963333\ncpu: 1.9229099265517242 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_intrusive>",
            "value": 3.266222525966124,
            "unit": "ns/iter",
            "extra": "iterations: 214582751\ncpu: 3.2660323242849993 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_intrusive>",
            "value": 1.927199445052767,
            "unit": "ns/iter",
            "extra": "iterations: 362199478\ncpu: 1.926993892575402 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_intrusive>",
            "value": 1.929249243753719,
            "unit": "ns/iter",
            "extra": "iterations: 361769391\ncpu: 1.9291225884834466 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_intrusive>",
            "value": 1.9710821033517214,
            "unit": "ns/iter",
            "extra": "iterations: 355099547\ncpu: 1.9709550657354156 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_intrusive>",
            "value": 1.9619156973407759,
            "unit": "ns/iter",
            "extra": "iterations: 356373100\ncpu: 1.961815594386889 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_intrusive>",
            "value": 1.8329999988964118e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.241999999734844e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_intrusive>",
            "value": 39.8405843435526,
            "unit": "ns/iter",
            "extra": "iterations: 17466095\ncpu: 39.838655349120685 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_intrusive>",
            "value": 27.16849215465725,
            "unit": "ns/iter",
            "extra": "iterations: 25674596\ncpu: 27.165709092365116 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_intrusive>",
            "value": 27.193258580622906,
            "unit": "ns/iter",
            "extra": "iterations: 25695568\ncpu: 27.190867390049416 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_intrusive>",
            "value": 2.481288004942269,
            "unit": "ns/iter",
            "extra": "iterations: 280267496\ncpu: 2.4811080875393436 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_intrusive>",
            "value": 2.479163352697933,
            "unit": "ns/iter",
            "extra": "iterations: 274852975\ncpu: 2.4789378321264266 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_intrusive>",
            "value": 2.013599648889001,
            "unit": "ns/iter",
            "extra": "iterations: 338707715\ncpu: 2.0134563867256436 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_intrusive>",
            "value": 3.5390675223685784,
            "unit": "ns/iter",
            "extra": "iterations: 199042351\ncpu: 3.5384634197774316 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert/threads:4",
            "value": 25.972499063964356,
            "unit": "ns/iter",
            "extra": "iterations: 27125004\ncpu: 25.932657853248617 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/emplace/threads:4",
            "value": 27.699730914948805,
            "unit": "ns/iter",
            "extra": "iterations: 26716460\ncpu: 26.830083626348706 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update_insert/threads:4",
            "value": 25.67900907525683,
            "unit": "ns/iter",
            "extra": "iterations: 27175868\ncpu: 25.655684337295128 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update/threads:4",
            "value": 26.137506166834402,
            "unit": "ns/iter",
            "extra": "iterations: 24875008\ncpu: 26.05868782032151 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/find/threads:4",
            "value": 24.13145735394657,
            "unit": "ns/iter",
            "extra": "iterations: 28792676\ncpu: 24.11018010274557 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert_overflow/threads:4",
            "value": 25.67047520530221,
            "unit": "ns/iter",
            "extra": "iterations: 27272928\ncpu: 25.52529138052211 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request/threads:4",
            "value": 41.67002158032288,
            "unit": "ns/iter",
            "extra": "iterations: 18433460\ncpu: 41.58428276623054 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request_hot/threads:4",
            "value": 45.357613610937015,
            "unit": "ns/iter",
            "extra": "iterations: 18448224\ncpu: 43.16348462594557 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/many_shards/threads:4",
            "value": 39.47682459071913,
            "unit": "ns/iter",
            "extra": "iterations: 18016424\ncpu: 39.32574588608705 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert/threads:4",
            "value": 60.98906536728162,
            "unit": "ns/iter",
            "extra": "iterations: 11317344\ncpu: 60.49591078966938 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/emplace/threads:4",
            "value": 60.58148822491346,
            "unit": "ns/iter",
            "extra": "iterations: 11402040\ncpu: 60.495935201069294 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update_insert/threads:4",
            "value": 62.25307829258631,
            "unit": "ns/iter",
            "extra": "iterations: 11087640\ncpu: 62.101738602624174 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update/threads:4",
            "value": 61.673306796278894,
            "unit": "ns/iter",
            "extra": "iterations: 10740704\ncpu: 61.624946837749164 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/find/threads:4",
            "value": 32.10502752561783,
            "unit": "ns/iter",
            "extra": "iterations: 21949008\ncpu: 31.870105291318875 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert_overflow/threads:4",
            "value": 63.69026383846161,
            "unit": "ns/iter",
            "extra": "iterations: 11027960\ncpu: 63.60081238959878 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request/threads:4",
            "value": 79.79439268451782,
            "unit": "ns/iter",
            "extra": "iterations: 8757096\ncpu: 79.59687834871283 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request_hot/threads:4",
            "value": 79.91755664468116,
            "unit": "ns/iter",
            "extra": "iterations: 8764636\ncpu: 79.85733999677794 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/many_shards/threads:4",
            "value": 76.23043945795493,
            "unit": "ns/iter",
            "extra": "iterations: 9247392\ncpu: 76.13052717998762 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert/threads:4",
            "value": 206.92919734493384,
            "unit": "ns/iter",
            "extra": "iterations: 2471348\ncpu: 206.55473935682082 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/emplace/threads:4",
            "value": 206.28691954832263,
            "unit": "ns/iter",
            "extra": "iterations: 2580232\ncpu: 205.62367376266957 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update_insert/threads:4",
            "value": 202.80396403485418,
            "unit": "ns/iter",
            "extra": "iterations: 2516880\ncpu: 202.78057754044698 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update/threads:4",
            "value": 194.0138537584211,
            "unit": "ns/iter",
            "extra": "iterations: 3004672\ncpu: 193.88648844199983 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/find/threads:4",
            "value": 50.544733448679565,
            "unit": "ns/iter",
            "extra": "iterations: 13996104\ncpu: 50.51274919077474 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert_overflow/threads:4",
            "value": 108.2368669709107,
            "unit": "ns/iter",
            "extra": "iterations: 6356264\ncpu: 108.20744339756806 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request/threads:4",
            "value": 142.79747813943737,
            "unit": "ns/iter",
            "extra": "iterations: 4774808\ncpu: 142.61587335030003 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request_hot/threads:4",
            "value": 142.2091803125437,
            "unit": "ns/iter",
            "extra": "iterations: 3637240\ncpu: 141.78758756639644 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/many_shards/threads:4",
            "value": 95.52496428384346,
            "unit": "ns/iter",
            "extra": "iterations: 7237620\ncpu: 95.40955687090515 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert/threads:4",
            "value": 66.72577126991833,
            "unit": "ns/iter",
            "extra": "iterations: 10629612\ncpu: 66.10348439811348 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/emplace/threads:4",
            "value": 65.21265315540766,
            "unit": "ns/iter",
            "extra": "iterations: 10643764\ncpu: 65.11838227529286 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update_insert/threads:4",
            "value": 67.49373683685431,
            "unit": "ns/iter",
            "extra": "iterations: 10335672\ncpu: 67.40848393795773 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update/threads:4",
            "value": 66.23083760575226,
            "unit": "ns/iter",
            "extra": "iterations: 10428916\ncpu: 66.19543996710694 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/find/threads:4",
            "value": 33.222659070454284,
            "unit": "ns/iter",
            "extra": "iterations: 21306408\ncpu: 33.188477335081565 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert_overflow/threads:4",
            "value": 68.47427719789735,
            "unit": "ns/iter",
            "extra": "iterations: 10280684\ncpu: 68.2986343126585 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request/threads:4",
            "value": 85.64030063697268,
            "unit": "ns/iter",
            "extra": "iterations: 8179300\ncpu: 85.59456090374468 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request_hot/threads:4",
            "value": 84.75180484005983,
            "unit": "ns/iter",
            "extra": "iterations: 8279404\ncpu: 83.94315496622708 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/many_shards/threads:4",
            "value": 80.08194199782068,
            "unit": "ns/iter",
            "extra": "iterations: 8798980\ncpu: 79.92528372606812 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert/threads:4",
            "value": 79.8501745889937,
            "unit": "ns/iter",
            "extra": "iterations: 8725636\ncpu: 79.74708617228602 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/emplace/threads:4",
            "value": 79.59490556495697,
            "unit": "ns/iter",
            "extra": "iterations: 8779580\ncpu: 79.56706175010666 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update_insert/threads:4",
            "value": 81.69571239879993,
            "unit": "ns/iter",
            "extra": "iterations: 8642968\ncpu: 81.5681866460689 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update/threads:4",
            "value": 81.2074802247386,
            "unit": "ns/iter",
            "extra": "iterations: 8605196\ncpu: 80.98055233140539 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/find/threads:4",
            "value": 33.02802124054543,
            "unit": "ns/iter",
            "extra": "iterations: 21129400\ncpu: 32.995625810482096 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert_overflow/threads:4",
            "value": 87.75983707711065,
            "unit": "ns/iter",
            "extra": "iterations: 8022200\ncpu: 87.61393420757408 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request/threads:4",
            "value": 108.2066640995666,
            "unit": "ns/iter",
            "extra": "iterations: 6461248\ncpu: 107.90317009964652 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request_hot/threads:4",
            "value": 108.19981363326058,
            "unit": "ns/iter",
            "extra": "iterations: 6437844\ncpu: 108.10011892180057 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/many_shards/threads:4",
            "value": 100.84476191130857,
            "unit": "ns/iter",
            "extra": "iterations: 6985296\ncpu: 99.99621805575597 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert/threads:4",
            "value": 91.18669285429384,
            "unit": "ns/iter",
            "extra": "iterations: 7621920\ncpu: 91.0082170896571 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/emplace/threads:4",
            "value": 87.62760746622484,
            "unit": "ns/iter",
            "extra": "iterations: 7778816\ncpu: 87.5667131090387 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update_insert/threads:4",
            "value": 69.41586737665435,
            "unit": "ns/iter",
            "extra": "iterations: 10136496\ncpu: 68.80888445080032 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update/threads:4",
            "value": 70.48577472383822,
            "unit": "ns/iter",
            "extra": "iterations: 9998892\ncpu: 70.35261116931763 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/find/threads:4",
            "value": 38.065860518915734,
            "unit": "ns/iter",
            "extra": "iterations: 18365616\ncpu: 38.037809404269346 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert_overflow/threads:4",
            "value": 75.58276048288405,
            "unit": "ns/iter",
            "extra": "iterations: 9406760\ncpu: 73.95366236621325 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request/threads:4",
            "value": 91.32033335624682,
            "unit": "ns/iter",
            "extra": "iterations: 7622116\ncpu: 91.17550100260866 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request_hot/threads:4",
            "value": 90.60819311768336,
            "unit": "ns/iter",
            "extra": "iterations: 7633480\ncpu: 90.51277661040561 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/many_shards/threads:4",
            "value": 81.9232891096729,
            "unit": "ns/iter",
            "extra": "iterations: 8553792\ncpu: 81.81603971665444 ns\nthreads: 4"
          },
          {
            "name": "insert<dm_cache_std>",
            "value": 0.9996902692479925,
            "unit": "ns/iter",
            "extra": "iterations: 698716542\ncpu: 0.9995453034515391 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_std>",
            "value": 1.001357534401989,
            "unit": "ns/iter",
            "extra": "iterations: 700775611\ncpu: 1.0013104265981654 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_std>",
            "value": 1.0003243265907313,
            "unit": "ns/iter",
            "extra": "iterations: 699816193\ncpu: 1.0002039378360026 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_std>",
            "value": 1.0012959668029082,
            "unit": "ns/iter",
            "extra": "iterations: 703566633\ncpu: 1.001171763925877 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_std>",
            "value": 1.2120000008053467e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.0319999996788454e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_std>",
            "value": 0.61454675765386,
            "unit": "ns/iter",
            "extra": "iterations: 1149567388\ncpu: 0.6144994076676084 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_std>",
            "value": 1.9727278627219182,
            "unit": "ns/iter",
            "extra": "iterations: 349879949\ncpu: 1.9723479324046667 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_std>",
            "value": 1.972168826793355,
            "unit": "ns/iter",
            "extra": "iterations: 348987947\ncpu: 1.971912196153871 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_std>",
            "value": 2.05503518986116,
            "unit": "ns/iter",
            "extra": "iterations: 334187309\ncpu: 2.0546219695015417 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_std>",
            "value": 2.0665695941760536,
            "unit": "ns/iter",
            "extra": "iterations: 342540574\ncpu: 2.0663372830104496 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_std>",
            "value": 3.1649999812088935e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 2.012999999578824e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_std>",
            "value": 16.79156480088952,
            "unit": "ns/iter",
            "extra": "iterations: 41948174\ncpu: 16.78824487092096 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_std>",
            "value": 29.858499267858093,
            "unit": "ns/iter",
            "extra": "iterations: 22746271\ncpu: 29.853797706006382 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_std>",
            "value": 30.0542728925985,
            "unit": "ns/iter",
            "extra": "iterations: 22806892\ncpu: 30.04785097417044 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_std>",
            "value": 29.69189076717099,
            "unit": "ns/iter",
            "extra": "iterations: 22823688\ncpu: 29.68908946704845 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_std>",
            "value": 31.246850147188255,
            "unit": "ns/iter",
            "extra": "iterations: 22768048\ncpu: 31.242156947315003 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_std>",
            "value": 2.993795116182651,
            "unit": "ns/iter",
            "extra": "iterations: 233083494\ncpu: 2.9933633824795782 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_std>",
            "value": 104.311227130632,
            "unit": "ns/iter",
            "extra": "iterations: 7671889\ncpu: 104.29599111770254 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_std>",
            "value": 3.9746453264053137,
            "unit": "ns/iter",
            "extra": "iterations: 179806732\ncpu: 3.9742196471264397 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_std>",
            "value": 4.14509163590603,
            "unit": "ns/iter",
            "extra": "iterations: 162991077\ncpu: 4.144867212577521 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_std>",
            "value": 4.199619521634118,
            "unit": "ns/iter",
            "extra": "iterations: 167238418\ncpu: 4.198900942724777 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_std>",
            "value": 4.073312277655591,
            "unit": "ns/iter",
            "extra": "iterations: 173379363\ncpu: 4.072848704606205 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_std>",
            "value": 2.975883780501045,
            "unit": "ns/iter",
            "extra": "iterations: 239560558\ncpu: 2.975289797079206 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_std>",
            "value": 16.59518179671955,
            "unit": "ns/iter",
            "extra": "iterations: 42319053\ncpu: 16.59366356803868 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_std>",
            "value": 1.9346665476555116,
            "unit": "ns/iter",
            "extra": "iterations: 357916047\ncpu: 1.9343732107099332 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_std>",
            "value": 1.9362137176905136,
            "unit": "ns/iter",
            "extra": "iterations: 358664703\ncpu: 1.9360158950461304 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_std>",
            "value": 1.9984060964372425,
            "unit": "ns/iter",
            "extra": "iterations: 345008326\ncpu: 1.9979954222901894 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_std>",
            "value": 1.9765789340069877,
            "unit": "ns/iter",
            "extra": "iterations: 350438063\ncpu: 1.9763351134605476 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_std>",
            "value": 1.651999980367691e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 1.111999999636737e-9 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_std>",
            "value": 16.609600880646823,
            "unit": "ns/iter",
            "extra": "iterations: 42256207\ncpu: 16.60650176197788 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_std>",
            "value": 27.792716302149575,
            "unit": "ns/iter",
            "extra": "iterations: 25066718\ncpu: 27.789813887881234 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_std>",
            "value": 27.80703934091069,
            "unit": "ns/iter",
            "extra": "iterations: 25065358\ncpu: 27.800693251618345 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_std>",
            "value": 4.122479125856074,
            "unit": "ns/iter",
            "extra": "iterations: 168407505\ncpu: 4.122129984646475 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_std>",
            "value": 4.041463905407427,
            "unit": "ns/iter",
            "extra": "iterations: 171446706\ncpu: 4.040927324669631 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_std>",
            "value": 2.895638846399829,
            "unit": "ns/iter",
            "extra": "iterations: 240449958\ncpu: 2.8952763593329345 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_std>",
            "value": 16.667420897695543,
            "unit": "ns/iter",
            "extra": "iterations: 40971973\ncpu: 16.66489600098093 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert/threads:4",
            "value": 25.326336156605745,
            "unit": "ns/iter",
            "extra": "iterations: 27659632\ncpu: 25.277498702802703 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/emplace/threads:4",
            "value": 25.273181308988526,
            "unit": "ns/iter",
            "extra": "iterations: 27612332\ncpu: 25.250662385198034 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update_insert/threads:4",
            "value": 25.38064317351809,
            "unit": "ns/iter",
            "extra": "iterations: 27639944\ncpu: 25.353355455423504 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update/threads:4",
            "value": 25.352620543595897,
            "unit": "ns/iter",
            "extra": "iterations: 26664544\ncpu: 25.325355273279754 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/find/threads:4",
            "value": 23.872094998179616,
            "unit": "ns/iter",
            "extra": "iterations: 29721496\ncpu: 23.833104531481183 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert_overflow/threads:4",
            "value": 27.85377836770324,
            "unit": "ns/iter",
            "extra": "iterations: 27583128\ncpu: 27.7925787097098 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request/threads:4",
            "value": 39.671181338380705,
            "unit": "ns/iter",
            "extra": "iterations: 18570564\ncpu: 39.64061263836682 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request_hot/threads:4",
            "value": 37.57758085281182,
            "unit": "ns/iter",
            "extra": "iterations: 18464788\ncpu: 37.524667545600856 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/many_shards/threads:4",
            "value": 37.86320347342842,
            "unit": "ns/iter",
            "extra": "iterations: 18544908\ncpu: 37.624706145751695 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert/threads:4",
            "value": 125.53847532580744,
            "unit": "ns/iter",
            "extra": "iterations: 4279288\ncpu: 125.43470362359342 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/emplace/threads:4",
            "value": 122.04459236989115,
            "unit": "ns/iter",
            "extra": "iterations: 5270072\ncpu: 121.88643836365044 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update_insert/threads:4",
            "value": 110.3988139318293,
            "unit": "ns/iter",
            "extra": "iterations: 6379060\ncpu: 109.365791511602 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update/threads:4",
            "value": 112.2740120302934,
            "unit": "ns/iter",
            "extra": "iterations: 6323372\ncpu: 112.06384109617464 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/find/threads:4",
            "value": 28.132436160924055,
            "unit": "ns/iter",
            "extra": "iterations: 24748792\ncpu: 28.068224703654213 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert_overflow/threads:4",
            "value": 110.41417294347454,
            "unit": "ns/iter",
            "extra": "iterations: 6351948\ncpu: 110.24328520951364 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request/threads:4",
            "value": 130.86166889019597,
            "unit": "ns/iter",
            "extra": "iterations: 5564728\ncpu: 129.86011930861676 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request_hot/threads:4",
            "value": 134.66622658687237,
            "unit": "ns/iter",
            "extra": "iterations: 5575504\ncpu: 134.36770379861628 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/many_shards/threads:4",
            "value": 126.48533444636384,
            "unit": "ns/iter",
            "extra": "iterations: 5570400\ncpu: 126.3102540212552 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert/threads:4",
            "value": 428.69673226649934,
            "unit": "ns/iter",
            "extra": "iterations: 2588828\ncpu: 428.1297146817017 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/emplace/threads:4",
            "value": 314.31535714801447,
            "unit": "ns/iter",
            "extra": "iterations: 2766920\ncpu: 311.8494383646797 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update_insert/threads:4",
            "value": 312.1279869718674,
            "unit": "ns/iter",
            "extra": "iterations: 2943784\ncpu: 309.9913264696051 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update/threads:4",
            "value": 311.82883604621776,
            "unit": "ns/iter",
            "extra": "iterations: 2920872\ncpu: 311.6442483614482 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/find/threads:4",
            "value": 54.34439220125561,
            "unit": "ns/iter",
            "extra": "iterations: 13155144\ncpu: 53.91948974484813 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert_overflow/threads:4",
            "value": 183.20146448202672,
            "unit": "ns/iter",
            "extra": "iterations: 4302272\ncpu: 183.0202960203353 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request/threads:4",
            "value": 214.47765744878186,
            "unit": "ns/iter",
            "extra": "iterations: 3683420\ncpu: 214.135928023413 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request_hot/threads:4",
            "value": 218.7771295560916,
            "unit": "ns/iter",
            "extra": "iterations: 3888416\ncpu: 218.45251845481553 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/many_shards/threads:4",
            "value": 151.45585806229064,
            "unit": "ns/iter",
            "extra": "iterations: 4773080\ncpu: 151.10842202519115 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert/threads:4",
            "value": 141.8220324981785,
            "unit": "ns/iter",
            "extra": "iterations: 5798480\ncpu: 141.01054207309488 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/emplace/threads:4",
            "value": 147.89475449230687,
            "unit": "ns/iter",
            "extra": "iterations: 5289936\ncpu: 146.74645553367756 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update_insert/threads:4",
            "value": 136.9055797499925,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 136.61168324999997 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update/threads:4",
            "value": 138.0128440015582,
            "unit": "ns/iter",
            "extra": "iterations: 5744160\ncpu: 124.19167902704649 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/find/threads:4",
            "value": 31.046469033690972,
            "unit": "ns/iter",
            "extra": "iterations: 22532812\ncpu: 30.991395703297062 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert_overflow/threads:4",
            "value": 122.06923350678333,
            "unit": "ns/iter",
            "extra": "iterations: 5822152\ncpu: 121.73047749354524 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request/threads:4",
            "value": 143.06360933303566,
            "unit": "ns/iter",
            "extra": "iterations: 5150392\ncpu: 142.35887792618507 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request_hot/threads:4",
            "value": 143.07655178717357,
            "unit": "ns/iter",
            "extra": "iterations: 5098444\ncpu: 142.9301082840179 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/many_shards/threads:4",
            "value": 134.25254132781794,
            "unit": "ns/iter",
            "extra": "iterations: 5361764\ncpu: 132.96322963860396 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert/threads:4",
            "value": 81.96586422841668,
            "unit": "ns/iter",
            "extra": "iterations: 8585832\ncpu: 81.67597910138483 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/emplace/threads:4",
            "value": 81.26073814867723,
            "unit": "ns/iter",
            "extra": "iterations: 8481164\ncpu: 81.19408927831142 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update_insert/threads:4",
            "value": 82.04028976951956,
            "unit": "ns/iter",
            "extra": "iterations: 8476668\ncpu: 81.90983756825206 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update/threads:4",
            "value": 83.45326353131604,
            "unit": "ns/iter",
            "extra": "iterations: 8686664\ncpu: 82.73039500549346 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/find/threads:4",
            "value": 27.45958754245607,
            "unit": "ns/iter",
            "extra": "iterations: 25407512\ncpu: 27.40236151418528 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert_overflow/threads:4",
            "value": 91.16523211484633,
            "unit": "ns/iter",
            "extra": "iterations: 7758056\ncpu: 90.9549856304207 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request/threads:4",
            "value": 107.43459704291074,
            "unit": "ns/iter",
            "extra": "iterations: 6622988\ncpu: 106.41521545864184 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request_hot/threads:4",
            "value": 103.53115365097979,
            "unit": "ns/iter",
            "extra": "iterations: 6755388\ncpu: 103.42677237784132 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/many_shards/threads:4",
            "value": 99.99809768200583,
            "unit": "ns/iter",
            "extra": "iterations: 6963084\ncpu: 99.93170583609229 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert/threads:4",
            "value": 156.22370924133938,
            "unit": "ns/iter",
            "extra": "iterations: 4567004\ncpu: 155.34337675202352 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/emplace/threads:4",
            "value": 146.71562842480415,
            "unit": "ns/iter",
            "extra": "iterations: 4803184\ncpu: 145.4348886072239 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update_insert/threads:4",
            "value": 126.00280167536664,
            "unit": "ns/iter",
            "extra": "iterations: 5037700\ncpu: 125.61914643587367 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update/threads:4",
            "value": 125.43005585498003,
            "unit": "ns/iter",
            "extra": "iterations: 5597352\ncpu: 124.9683359202707 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/find/threads:4",
            "value": 35.783364459364435,
            "unit": "ns/iter",
            "extra": "iterations: 19578468\ncpu: 35.743501585517286 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert_overflow/threads:4",
            "value": 125.89167380332995,
            "unit": "ns/iter",
            "extra": "iterations: 5441380\ncpu: 125.64878045642831 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request/threads:4",
            "value": 143.79329106667936,
            "unit": "ns/iter",
            "extra": "iterations: 4898424\ncpu: 143.5930278799879 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request_hot/threads:4",
            "value": 143.25267586523483,
            "unit": "ns/iter",
            "extra": "iterations: 4852636\ncpu: 143.20277329682253 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/many_shards/threads:4",
            "value": 132.4482844331717,
            "unit": "ns/iter",
            "extra": "iterations: 5310548\ncpu: 132.3316202019077 ns\nthreads: 4"
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "wstux1@gmail.com",
            "name": "wstux",
            "username": "wstux"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "06e17d5bb5f9dfcb910bf97a74001a697eb59ca3",
          "message": "* all: #44 Merge remote-tracking branch 'wstux/feature/cxx_std_build' into master;",
          "timestamp": "2026-11-22T13:02:09+03:00",
          "tree_id": "d767b064ab4973eeccb365f4b7351329d894bb21",
          "url": "https://github.com/wstux/thread_safe_lru_cache/commit/06e17d5bb5f9dfcb910bf97a74001a697eb59ca3"
        },
        "date": 1795341930983,
        "tool": "googlecpp",
        "benches": [
          {
            "name": "insert<dm_cache_intrusive>",
            "value": 0.5956559179619147,
            "unit": "ns/iter",
            "extra": "iterations: 1184338637\ncpu: 0.5956351494087075 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_intrusive>",
            "value": 0.5934050761852796,
            "unit": "ns/iter",
            "extra": "iterations: 1195527048\ncpu: 0.5933719075505183 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_intrusive>",
            "value": 0.5880361477894558,
            "unit": "ns/iter",
            "extra": "iterations: 1191361039\ncpu: 0.5880033483283987 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_intrusive>",
            "value": 0.5831155201553055,
            "unit": "ns/iter",
            "extra": "iterations: 1186676746\ncpu: 0.5831048365381888 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_intrusive>",
            "value": 1.0320000001229345e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 8.510000002459606e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_intrusive>",
            "value": 1.562126908447154,
            "unit": "ns/iter",
            "extra": "iterations: 448270334\ncpu: 1.5620404115343494 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_intrusive>",
            "value": 1.5864241089316786,
            "unit": "ns/iter",
            "extra": "iterations: 435763249\ncpu: 1.5863738775272438 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_intrusive>",
            "value": 1.585720775243906,
            "unit": "ns/iter",
            "extra": "iterations: 437178944\ncpu: 1.5855523407824517 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_intrusive>",
            "value": 1.5963215310852255,
            "unit": "ns/iter",
            "extra": "iterations: 436152442\ncpu: 1.5962621367141168 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_intrusive>",
            "value": 1.6037858317476885,
            "unit": "ns/iter",
            "extra": "iterations: 442396681\ncpu: 1.6031683858857873 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_intrusive>",
            "value": 1.5751076058164903,
            "unit": "ns/iter",
            "extra": "iterations: 444985007\ncpu: 1.574724678308096 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_intrusive>",
            "value": 1.8461738037150464,
            "unit": "ns/iter",
            "extra": "iterations: 380661587\ncpu: 1.8457829158370007 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_intrusive>",
            "value": 94.83328232899791,
            "unit": "ns/iter",
            "extra": "iterations: 7202002\ncpu: 94.82687105613148 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_intrusive>",
            "value": 94.83027805636581,
            "unit": "ns/iter",
            "extra": "iterations: 7345597\ncpu: 94.82863884310555 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_intrusive>",
            "value": 95.00659504021066,
            "unit": "ns/iter",
            "extra": "iterations: 7376604\ncpu: 95.00115093612168 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_intrusive>",
            "value": 95.5131531023057,
            "unit": "ns/iter",
            "extra": "iterations: 7363586\ncpu: 95.50744623611368 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_intrusive>",
            "value": 1.6055863019350662,
            "unit": "ns/iter",
            "extra": "iterations: 420321033\ncpu: 1.6054043148490236 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_intrusive>",
            "value": 74.98958047363806,
            "unit": "ns/iter",
            "extra": "iterations: 9649191\ncpu: 74.98324346569575 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_intrusive>",
            "value": 1.857387283625938,
            "unit": "ns/iter",
            "extra": "iterations: 370344485\ncpu: 1.8572142231306614 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_intrusive>",
            "value": 1.857766532813241,
            "unit": "ns/iter",
            "extra": "iterations: 372766551\ncpu: 1.8575866803027643 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_intrusive>",
            "value": 1.9310429611860818,
            "unit": "ns/iter",
            "extra": "iterations: 364324229\ncpu: 1.9308876380000521 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_intrusive>",
            "value": 1.9154782981532037,
            "unit": "ns/iter",
            "extra": "iterations: 369595019\ncpu: 1.9152859849553288 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_intrusive>",
            "value": 1.6288089825584957,
            "unit": "ns/iter",
            "extra": "iterations: 427785826\ncpu: 1.6286880879498813 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_intrusive>",
            "value": 1.893094911513289,
            "unit": "ns/iter",
            "extra": "iterations: 359981817\ncpu: 1.8929403037042845 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_intrusive>",
            "value": 1.574579151651253,
            "unit": "ns/iter",
            "extra": "iterations: 441699547\ncpu: 1.5745513839071257 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_intrusive>",
            "value": 1.573188466301095,
            "unit": "ns/iter",
            "extra": "iterations: 444131967\ncpu: 1.5730967908464015 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_intrusive>",
            "value": 1.5902740428590851,
            "unit": "ns/iter",
            "extra": "iterations: 436836451\ncpu: 1.5902130291778234 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_intrusive>",
            "value": 1.6028158707801494,
            "unit": "ns/iter",
            "extra": "iterations: 444662450\ncpu: 1.6026723259407154 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_intrusive>",
            "value": 8.509999815942138e-10,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 7.20999999259675e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_intrusive>",
            "value": 30.780648503676524,
            "unit": "ns/iter",
            "extra": "iterations: 23041288\ncpu: 30.77692731413286 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_intrusive>",
            "value": 22.202969299991874,
            "unit": "ns/iter",
            "extra": "iterations: 31459536\ncpu: 22.19997246621822 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_intrusive>",
            "value": 22.299953501719106,
            "unit": "ns/iter",
            "extra": "iterations: 31500089\ncpu: 22.299145694477264 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_intrusive>",
            "value": 1.9081842311177277,
            "unit": "ns/iter",
            "extra": "iterations: 368137722\ncpu: 1.908129091427363 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_intrusive>",
            "value": 1.8814810393515897,
            "unit": "ns/iter",
            "extra": "iterations: 369088775\ncpu: 1.8813623009803069 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_intrusive>",
            "value": 1.6018671329234562,
            "unit": "ns/iter",
            "extra": "iterations: 433708918\ncpu: 1.6017951030465163 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_intrusive>",
            "value": 2.093942376834622,
            "unit": "ns/iter",
            "extra": "iterations: 327432550\ncpu: 2.093851423751247 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert/threads:4",
            "value": 18.60624441226658,
            "unit": "ns/iter",
            "extra": "iterations: 37242288\ncpu: 18.582786481861696 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/emplace/threads:4",
            "value": 18.62072900296564,
            "unit": "ns/iter",
            "extra": "iterations: 37459052\ncpu: 18.607358376287795 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update_insert/threads:4",
            "value": 18.29408574353527,
            "unit": "ns/iter",
            "extra": "iterations: 38404388\ncpu: 18.17297283841628 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/update/threads:4",
            "value": 27.324246625290737,
            "unit": "ns/iter",
            "extra": "iterations: 37935704\ncpu: 27.30279053210664 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/find/threads:4",
            "value": 18.186386401719904,
            "unit": "ns/iter",
            "extra": "iterations: 37315072\ncpu: 18.171913911890623 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/insert_overflow/threads:4",
            "value": 18.463433473700483,
            "unit": "ns/iter",
            "extra": "iterations: 37662068\ncpu: 18.447123615198183 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request/threads:4",
            "value": 31.42318625217144,
            "unit": "ns/iter",
            "extra": "iterations: 22480468\ncpu: 31.232151261263777 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/request_hot/threads:4",
            "value": 31.046203664982052,
            "unit": "ns/iter",
            "extra": "iterations: 22534576\ncpu: 31.02380098032463 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_intrusive>/many_shards/threads:4",
            "value": 32.19651096188442,
            "unit": "ns/iter",
            "extra": "iterations: 21243792\ncpu: 32.17543346310301 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert/threads:4",
            "value": 45.76360257726188,
            "unit": "ns/iter",
            "extra": "iterations: 15165876\ncpu: 45.66919761179638 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/emplace/threads:4",
            "value": 45.863959415121656,
            "unit": "ns/iter",
            "extra": "iterations: 15108484\ncpu: 45.77077164062256 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update_insert/threads:4",
            "value": 45.98667923867182,
            "unit": "ns/iter",
            "extra": "iterations: 15163848\ncpu: 45.95358460464651 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/update/threads:4",
            "value": 46.10083859095335,
            "unit": "ns/iter",
            "extra": "iterations: 15143736\ncpu: 45.72172520704271 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/find/threads:4",
            "value": 24.288471374378695,
            "unit": "ns/iter",
            "extra": "iterations: 28733000\ncpu: 24.278571015905065 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/insert_overflow/threads:4",
            "value": 50.292923706390596,
            "unit": "ns/iter",
            "extra": "iterations: 13622216\ncpu: 50.16286058009944 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request/threads:4",
            "value": 62.709848358719114,
            "unit": "ns/iter",
            "extra": "iterations: 11387928\ncpu: 62.631767780758736 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/request_hot/threads:4",
            "value": 61.28290693596191,
            "unit": "ns/iter",
            "extra": "iterations: 11450352\ncpu: 61.24864650449169 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_intrusive>/many_shards/threads:4",
            "value": 59.371713489298315,
            "unit": "ns/iter",
            "extra": "iterations: 11781340\ncpu: 59.21832592896903 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert/threads:4",
            "value": 135.18237162763447,
            "unit": "ns/iter",
            "extra": "iterations: 5005828\ncpu: 134.3343193174036 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/emplace/threads:4",
            "value": 139.29814762929715,
            "unit": "ns/iter",
            "extra": "iterations: 3783260\ncpu: 139.1383219762848 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update_insert/threads:4",
            "value": 137.4551621576329,
            "unit": "ns/iter",
            "extra": "iterations: 3766952\ncpu: 137.37799313609523 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/update/threads:4",
            "value": 129.3059902292344,
            "unit": "ns/iter",
            "extra": "iterations: 4257600\ncpu: 129.2753938838783 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/find/threads:4",
            "value": 36.29328438597928,
            "unit": "ns/iter",
            "extra": "iterations: 19457640\ncpu: 36.27838823207747 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/insert_overflow/threads:4",
            "value": 84.48921430155954,
            "unit": "ns/iter",
            "extra": "iterations: 8224224\ncpu: 84.3982173880478 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request/threads:4",
            "value": 111.31370578900572,
            "unit": "ns/iter",
            "extra": "iterations: 6578680\ncpu: 111.0563845634687 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/request_hot/threads:4",
            "value": 110.9226901455914,
            "unit": "ns/iter",
            "extra": "iterations: 6334988\ncpu: 108.35363145123566 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_intrusive>/many_shards/threads:4",
            "value": 76.12248693126483,
            "unit": "ns/iter",
            "extra": "iterations: 8750656\ncpu: 76.05162184412238 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert/threads:4",
            "value": 51.59810606470989,
            "unit": "ns/iter",
            "extra": "iterations: 14027512\ncpu: 51.5466739219328 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/emplace/threads:4",
            "value": 53.32002842552688,
            "unit": "ns/iter",
            "extra": "iterations: 13203624\ncpu: 52.8605738848668 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update_insert/threads:4",
            "value": 52.771740484853076,
            "unit": "ns/iter",
            "extra": "iterations: 13033012\ncpu: 52.707005410568236 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/update/threads:4",
            "value": 50.69903607510348,
            "unit": "ns/iter",
            "extra": "iterations: 13908760\ncpu: 50.64384373589021 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/find/threads:4",
            "value": 25.658647650431384,
            "unit": "ns/iter",
            "extra": "iterations: 27975252\ncpu: 25.636937676200343 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/insert_overflow/threads:4",
            "value": 54.56575708002067,
            "unit": "ns/iter",
            "extra": "iterations: 13220584\ncpu: 54.20221050749354 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request/threads:4",
            "value": 66.33775653241123,
            "unit": "ns/iter",
            "extra": "iterations: 10914800\ncpu: 65.98836451423756 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/request_hot/threads:4",
            "value": 65.4397025781177,
            "unit": "ns/iter",
            "extra": "iterations: 10377044\ncpu: 65.40612316956549 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_intrusive>/many_shards/threads:4",
            "value": 60.95771308518099,
            "unit": "ns/iter",
            "extra": "iterations: 11447300\ncpu: 60.85500808050799 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert/threads:4",
            "value": 57.838895451472524,
            "unit": "ns/iter",
            "extra": "iterations: 12170004\ncpu: 57.43055975988175 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/emplace/threads:4",
            "value": 57.86702404726979,
            "unit": "ns/iter",
            "extra": "iterations: 12117800\ncpu: 57.73215121556722 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update_insert/threads:4",
            "value": 57.58095410412402,
            "unit": "ns/iter",
            "extra": "iterations: 12103920\ncpu: 57.53676296604731 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/update/threads:4",
            "value": 59.51481949151836,
            "unit": "ns/iter",
            "extra": "iterations: 12254604\ncpu: 59.39671669521108 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/find/threads:4",
            "value": 26.297560217406673,
            "unit": "ns/iter",
            "extra": "iterations: 26680492\ncpu: 26.284471440781505 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/insert_overflow/threads:4",
            "value": 66.76307727094766,
            "unit": "ns/iter",
            "extra": "iterations: 10047968\ncpu: 66.73947190118423 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request/threads:4",
            "value": 76.23579301526686,
            "unit": "ns/iter",
            "extra": "iterations: 9228700\ncpu: 76.0926099017195 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/request_hot/threads:4",
            "value": 78.89302933171537,
            "unit": "ns/iter",
            "extra": "iterations: 9198372\ncpu: 78.22335202359733 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_intrusive>/many_shards/threads:4",
            "value": 72.79196449516527,
            "unit": "ns/iter",
            "extra": "iterations: 9736588\ncpu: 72.73585633899684 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert/threads:4",
            "value": 64.89046279616605,
            "unit": "ns/iter",
            "extra": "iterations: 10223248\ncpu: 64.82945664626338 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/emplace/threads:4",
            "value": 64.47656519856791,
            "unit": "ns/iter",
            "extra": "iterations: 10688548\ncpu: 64.34892232321926 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update_insert/threads:4",
            "value": 50.42129361742717,
            "unit": "ns/iter",
            "extra": "iterations: 13604424\ncpu: 50.391406354286 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/update/threads:4",
            "value": 49.85700711675896,
            "unit": "ns/iter",
            "extra": "iterations: 13839444\ncpu: 49.82418180961607 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/find/threads:4",
            "value": 28.249175205367514,
            "unit": "ns/iter",
            "extra": "iterations: 24774652\ncpu: 28.234770119071726 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/insert_overflow/threads:4",
            "value": 58.40591371335882,
            "unit": "ns/iter",
            "extra": "iterations: 11436672\ncpu: 57.99127281083162 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request/threads:4",
            "value": 68.40695149938615,
            "unit": "ns/iter",
            "extra": "iterations: 10121356\ncpu: 68.39309574724966 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/request_hot/threads:4",
            "value": 68.48384266549279,
            "unit": "ns/iter",
            "extra": "iterations: 10134840\ncpu: 68.42760517186257 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_intrusive>/many_shards/threads:4",
            "value": 62.95989340859276,
            "unit": "ns/iter",
            "extra": "iterations: 11141236\ncpu: 62.861109306005055 ns\nthreads: 4"
          },
          {
            "name": "insert<dm_cache_std>",
            "value": 0.5881256545600979,
            "unit": "ns/iter",
            "extra": "iterations: 1200442641\ncpu: 0.5880915887925379 ns\nthreads: 1"
          },
          {
            "name": "emplace<dm_cache_std>",
            "value": 0.5894568307650835,
            "unit": "ns/iter",
            "extra": "iterations: 1201324869\ncpu: 0.5894361402752248 ns\nthreads: 1"
          },
          {
            "name": "update_insert<dm_cache_std>",
            "value": 0.5975185100855174,
            "unit": "ns/iter",
            "extra": "iterations: 1187138897\ncpu: 0.5974864590760689 ns\nthreads: 1"
          },
          {
            "name": "update<dm_cache_std>",
            "value": 0.5984202084241534,
            "unit": "ns/iter",
            "extra": "iterations: 1124633165\ncpu: 0.5983882184374318 ns\nthreads: 1"
          },
          {
            "name": "find<dm_cache_std>",
            "value": 1.0020000047461509e-9,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 8.209999999841955e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<dm_cache_std>",
            "value": 0.4301957102502089,
            "unit": "ns/iter",
            "extra": "iterations: 1586336483\ncpu: 0.4301796606918232 ns\nthreads: 1"
          },
          {
            "name": "insert<fifo_cache_std>",
            "value": 1.637445091486742,
            "unit": "ns/iter",
            "extra": "iterations: 417320305\ncpu: 1.6373562316839592 ns\nthreads: 1"
          },
          {
            "name": "emplace<fifo_cache_std>",
            "value": 1.6788993766604972,
            "unit": "ns/iter",
            "extra": "iterations: 423602538\ncpu: 1.6788442518727302 ns\nthreads: 1"
          },
          {
            "name": "update_insert<fifo_cache_std>",
            "value": 1.6692886528073119,
            "unit": "ns/iter",
            "extra": "iterations: 411033955\ncpu: 1.6691932543626469 ns\nthreads: 1"
          },
          {
            "name": "update<fifo_cache_std>",
            "value": 1.6392475154164696,
            "unit": "ns/iter",
            "extra": "iterations: 430739190\ncpu: 1.6392103606825283 ns\nthreads: 1"
          },
          {
            "name": "find<fifo_cache_std>",
            "value": 8.81000005392707e-10,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 7.41000000026304e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<fifo_cache_std>",
            "value": 11.784038099793921,
            "unit": "ns/iter",
            "extra": "iterations: 59415545\ncpu: 11.783278079835842 ns\nthreads: 1"
          },
          {
            "name": "insert<lfu_cache_std>",
            "value": 22.155411757069952,
            "unit": "ns/iter",
            "extra": "iterations: 31629293\ncpu: 22.154483693328185 ns\nthreads: 1"
          },
          {
            "name": "emplace<lfu_cache_std>",
            "value": 21.943678481044476,
            "unit": "ns/iter",
            "extra": "iterations: 31367034\ncpu: 21.94207632127416 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lfu_cache_std>",
            "value": 22.07717393983234,
            "unit": "ns/iter",
            "extra": "iterations: 32216186\ncpu: 22.075551059954762 ns\nthreads: 1"
          },
          {
            "name": "update<lfu_cache_std>",
            "value": 21.88449545765484,
            "unit": "ns/iter",
            "extra": "iterations: 32075613\ncpu: 21.881808494197788 ns\nthreads: 1"
          },
          {
            "name": "find<lfu_cache_std>",
            "value": 2.3682677531026157,
            "unit": "ns/iter",
            "extra": "iterations: 296222629\ncpu: 2.3681755386756778 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lfu_cache_std>",
            "value": 40.87683097371642,
            "unit": "ns/iter",
            "extra": "iterations: 16690641\ncpu: 40.87361090565666 ns\nthreads: 1"
          },
          {
            "name": "insert<lru_cache_std>",
            "value": 2.824526575418418,
            "unit": "ns/iter",
            "extra": "iterations: 258584376\ncpu: 2.824477871779841 ns\nthreads: 1"
          },
          {
            "name": "emplace<lru_cache_std>",
            "value": 2.674827622499094,
            "unit": "ns/iter",
            "extra": "iterations: 257507214\ncpu: 2.674683110042879 ns\nthreads: 1"
          },
          {
            "name": "update_insert<lru_cache_std>",
            "value": 2.9588094304092842,
            "unit": "ns/iter",
            "extra": "iterations: 238466598\ncpu: 2.9587339439463034 ns\nthreads: 1"
          },
          {
            "name": "update<lru_cache_std>",
            "value": 2.824887673688681,
            "unit": "ns/iter",
            "extra": "iterations: 247559763\ncpu: 2.824756897186082 ns\nthreads: 1"
          },
          {
            "name": "find<lru_cache_std>",
            "value": 2.4492280430872273,
            "unit": "ns/iter",
            "extra": "iterations: 292174921\ncpu: 2.449185363174962 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<lru_cache_std>",
            "value": 11.781836163689196,
            "unit": "ns/iter",
            "extra": "iterations: 59141383\ncpu: 11.781037721082738 ns\nthreads: 1"
          },
          {
            "name": "insert<rr_cache_std>",
            "value": 1.5821733306490218,
            "unit": "ns/iter",
            "extra": "iterations: 441177140\ncpu: 1.5821457657574955 ns\nthreads: 1"
          },
          {
            "name": "emplace<rr_cache_std>",
            "value": 1.5819920542076316,
            "unit": "ns/iter",
            "extra": "iterations: 440341230\ncpu: 1.581849523834055 ns\nthreads: 1"
          },
          {
            "name": "update_insert<rr_cache_std>",
            "value": 1.5840466917327016,
            "unit": "ns/iter",
            "extra": "iterations: 424148063\ncpu: 1.5839268232140933 ns\nthreads: 1"
          },
          {
            "name": "update<rr_cache_std>",
            "value": 1.5819617902465737,
            "unit": "ns/iter",
            "extra": "iterations: 441383691\ncpu: 1.5817446413080085 ns\nthreads: 1"
          },
          {
            "name": "find<rr_cache_std>",
            "value": 9.419999855708738e-10,
            "unit": "ns/iter",
            "extra": "iterations: 1000000000000\ncpu: 7.709999998439798e-10 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<rr_cache_std>",
            "value": 26.41701124618584,
            "unit": "ns/iter",
            "extra": "iterations: 67499417\ncpu: 26.415621930482722 ns\nthreads: 1"
          },
          {
            "name": "insert<ttl_cache_std>",
            "value": 23.3550360595936,
            "unit": "ns/iter",
            "extra": "iterations: 30785011\ncpu: 23.354112525735314 ns\nthreads: 1"
          },
          {
            "name": "emplace<ttl_cache_std>",
            "value": 22.73933242646793,
            "unit": "ns/iter",
            "extra": "iterations: 30863087\ncpu: 22.737802929434864 ns\nthreads: 1"
          },
          {
            "name": "update_insert<ttl_cache_std>",
            "value": 2.856906517646836,
            "unit": "ns/iter",
            "extra": "iterations: 243341831\ncpu: 2.85681178259893 ns\nthreads: 1"
          },
          {
            "name": "update<ttl_cache_std>",
            "value": 2.856667156043756,
            "unit": "ns/iter",
            "extra": "iterations: 241633725\ncpu: 2.856400086535928 ns\nthreads: 1"
          },
          {
            "name": "find<ttl_cache_std>",
            "value": 2.419623699647881,
            "unit": "ns/iter",
            "extra": "iterations: 288241346\ncpu: 2.4194015698219693 ns\nthreads: 1"
          },
          {
            "name": "insert_overflow<ttl_cache_std>",
            "value": 11.347562369151593,
            "unit": "ns/iter",
            "extra": "iterations: 62010784\ncpu: 11.34681611185563 ns\nthreads: 1"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert/threads:4",
            "value": 18.37704897773913,
            "unit": "ns/iter",
            "extra": "iterations: 37061408\ncpu: 18.34209229179852 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/emplace/threads:4",
            "value": 18.465540865924357,
            "unit": "ns/iter",
            "extra": "iterations: 36090460\ncpu: 18.452687801707157 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update_insert/threads:4",
            "value": 19.010325085093406,
            "unit": "ns/iter",
            "extra": "iterations: 38597648\ncpu: 18.953728242715727 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/update/threads:4",
            "value": 18.281557452281017,
            "unit": "ns/iter",
            "extra": "iterations: 28087692\ncpu: 18.133069388542143 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/find/threads:4",
            "value": 18.068947606086773,
            "unit": "ns/iter",
            "extra": "iterations: 38816112\ncpu: 18.054338904421957 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/insert_overflow/threads:4",
            "value": 18.63937133248827,
            "unit": "ns/iter",
            "extra": "iterations: 38138252\ncpu: 18.55568514257025 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request/threads:4",
            "value": 31.580939231519938,
            "unit": "ns/iter",
            "extra": "iterations: 22090400\ncpu: 31.54556821062543 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/request_hot/threads:4",
            "value": 31.072397210936252,
            "unit": "ns/iter",
            "extra": "iterations: 22649588\ncpu: 30.99415057792662 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_dm_cache_std>/many_shards/threads:4",
            "value": 31.150237008218493,
            "unit": "ns/iter",
            "extra": "iterations: 22647316\ncpu: 30.989699662423583 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert/threads:4",
            "value": 69.33589830400162,
            "unit": "ns/iter",
            "extra": "iterations: 9983480\ncpu: 69.26698275551209 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/emplace/threads:4",
            "value": 71.7262763195563,
            "unit": "ns/iter",
            "extra": "iterations: 9769732\ncpu: 71.59287296724207 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update_insert/threads:4",
            "value": 74.08458407813903,
            "unit": "ns/iter",
            "extra": "iterations: 9483416\ncpu: 73.87177626711724 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/update/threads:4",
            "value": 72.34007872787554,
            "unit": "ns/iter",
            "extra": "iterations: 9772396\ncpu: 72.23152162478883 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/find/threads:4",
            "value": 22.10895930721739,
            "unit": "ns/iter",
            "extra": "iterations: 31431716\ncpu: 22.091624014419068 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/insert_overflow/threads:4",
            "value": 81.45223265304259,
            "unit": "ns/iter",
            "extra": "iterations: 8645320\ncpu: 81.24362337079482 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request/threads:4",
            "value": 94.20938315477615,
            "unit": "ns/iter",
            "extra": "iterations: 7443488\ncpu: 93.66621911662918 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/request_hot/threads:4",
            "value": 94.47637835835438,
            "unit": "ns/iter",
            "extra": "iterations: 7474036\ncpu: 94.11247698030881 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_fifo_cache_std>/many_shards/threads:4",
            "value": 90.79030302889126,
            "unit": "ns/iter",
            "extra": "iterations: 7719792\ncpu: 90.47731933710129 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert/threads:4",
            "value": 214.5480777377998,
            "unit": "ns/iter",
            "extra": "iterations: 3871064\ncpu: 214.12639806523498 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/emplace/threads:4",
            "value": 212.86824233898173,
            "unit": "ns/iter",
            "extra": "iterations: 3795840\ncpu: 211.7008290655032 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update_insert/threads:4",
            "value": 243.5964024253102,
            "unit": "ns/iter",
            "extra": "iterations: 3737240\ncpu: 242.4395765324143 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/update/threads:4",
            "value": 201.78067732472581,
            "unit": "ns/iter",
            "extra": "iterations: 4139344\ncpu: 201.0786117317141 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/find/threads:4",
            "value": 39.63895223133945,
            "unit": "ns/iter",
            "extra": "iterations: 17388724\ncpu: 39.42145220086304 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/insert_overflow/threads:4",
            "value": 129.12332349998934,
            "unit": "ns/iter",
            "extra": "iterations: 4000000\ncpu: 129.0124755000001 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request/threads:4",
            "value": 155.84008841008938,
            "unit": "ns/iter",
            "extra": "iterations: 4944232\ncpu: 155.46871627383194 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/request_hot/threads:4",
            "value": 156.45659013238722,
            "unit": "ns/iter",
            "extra": "iterations: 5055164\ncpu: 155.49463360634792 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lfu_cache_std>/many_shards/threads:4",
            "value": 118.1540374634613,
            "unit": "ns/iter",
            "extra": "iterations: 6066284\ncpu: 117.3737858300073 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert/threads:4",
            "value": 84.88345822159783,
            "unit": "ns/iter",
            "extra": "iterations: 7730644\ncpu: 84.75845143043715 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/emplace/threads:4",
            "value": 94.77568195467248,
            "unit": "ns/iter",
            "extra": "iterations: 7845976\ncpu: 94.62678601107123 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update_insert/threads:4",
            "value": 89.83292103959273,
            "unit": "ns/iter",
            "extra": "iterations: 8360544\ncpu: 89.68785320668128 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/update/threads:4",
            "value": 92.56237399220814,
            "unit": "ns/iter",
            "extra": "iterations: 7451920\ncpu: 92.39151748811048 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/find/threads:4",
            "value": 23.191080973252372,
            "unit": "ns/iter",
            "extra": "iterations: 30268168\ncpu: 23.14267328633831 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/insert_overflow/threads:4",
            "value": 86.83311629961624,
            "unit": "ns/iter",
            "extra": "iterations: 8141592\ncpu: 86.78213880037227 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request/threads:4",
            "value": 106.4943540133097,
            "unit": "ns/iter",
            "extra": "iterations: 6991692\ncpu: 97.69492792302624 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/request_hot/threads:4",
            "value": 100.19959769147708,
            "unit": "ns/iter",
            "extra": "iterations: 7033408\ncpu: 100.09064226048028 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_lru_cache_std>/many_shards/threads:4",
            "value": 95.7686545062591,
            "unit": "ns/iter",
            "extra": "iterations: 7414884\ncpu: 95.68859539812092 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert/threads:4",
            "value": 59.82808458338305,
            "unit": "ns/iter",
            "extra": "iterations: 11791252\ncpu: 59.54809624966036 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/emplace/threads:4",
            "value": 59.88288012435274,
            "unit": "ns/iter",
            "extra": "iterations: 11649372\ncpu: 59.77148141547891 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update_insert/threads:4",
            "value": 63.71612071592205,
            "unit": "ns/iter",
            "extra": "iterations: 11697148\ncpu: 63.635766427850676 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/update/threads:4",
            "value": 59.09725748044224,
            "unit": "ns/iter",
            "extra": "iterations: 11231424\ncpu: 59.043158641326244 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/find/threads:4",
            "value": 21.428728540845118,
            "unit": "ns/iter",
            "extra": "iterations: 32521784\ncpu: 21.41743746284026 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/insert_overflow/threads:4",
            "value": 67.02128782668167,
            "unit": "ns/iter",
            "extra": "iterations: 10491536\ncpu: 66.83781345267269 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request/threads:4",
            "value": 80.38196850686963,
            "unit": "ns/iter",
            "extra": "iterations: 8798872\ncpu: 79.74998761204833 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/request_hot/threads:4",
            "value": 79.8484244891501,
            "unit": "ns/iter",
            "extra": "iterations: 8867600\ncpu: 79.77274516216339 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_rr_cache_std>/many_shards/threads:4",
            "value": 78.53998070677451,
            "unit": "ns/iter",
            "extra": "iterations: 8950292\ncpu: 78.35448742901349 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert/threads:4",
            "value": 108.131827424837,
            "unit": "ns/iter",
            "extra": "iterations: 6180588\ncpu: 108.08033944343164 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/emplace/threads:4",
            "value": 109.89945272013755,
            "unit": "ns/iter",
            "extra": "iterations: 6430348\ncpu: 109.76392335220422 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update_insert/threads:4",
            "value": 102.02551732381741,
            "unit": "ns/iter",
            "extra": "iterations: 7813868\ncpu: 101.83538536868038 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/update/threads:4",
            "value": 96.65952907830975,
            "unit": "ns/iter",
            "extra": "iterations: 7678644\ncpu: 95.90173551476006 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/find/threads:4",
            "value": 27.481143822707836,
            "unit": "ns/iter",
            "extra": "iterations: 26247844\ncpu: 27.46373957419133 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/insert_overflow/threads:4",
            "value": 92.72254426641027,
            "unit": "ns/iter",
            "extra": "iterations: 7445148\ncpu: 92.49378064747677 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request/threads:4",
            "value": 105.20325064398568,
            "unit": "ns/iter",
            "extra": "iterations: 6739280\ncpu: 104.38868632851003 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/request_hot/threads:4",
            "value": 105.1991339583501,
            "unit": "ns/iter",
            "extra": "iterations: 6706144\ncpu: 104.7881307052159 ns\nthreads: 4"
          },
          {
            "name": "cache_fixture<thread_safe_ttl_cache_std>/many_shards/threads:4",
            "value": 96.49462487170057,
            "unit": "ns/iter",
            "extra": "iterations: 7287640\ncpu: 96.29376656914994 ns\nthreads: 4"
          }
        ]
      }
    ]
  }
}