time perf record -F 99 -a -g -- ./jsminify test/comp/phaser.js > phaser.min.js
perf script > out.perf
../../FlameGraph/stackcollapse-perf.pl out.perf > out-folded.perf
DAY=$(date +%F)
DATE=$(date +%s)
VERSION=$(head package.json -n 3 | tail -n 1 | sed -En "s/.* \"(.*)\".*/\1/p")

# --flamechart \
../../FlameGraph/flamegraph.pl --inverted \
  --width 1920 \
  --title "jsminify-$VERSION" \
  --subtitle "$DAY" \
  --cp \
  out-folded.perf > "perf/perf-graph-$DATE.svg"

