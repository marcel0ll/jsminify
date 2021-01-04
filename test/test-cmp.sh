if [ ! -d ./comp ]; then
  mkdir comp
  mkdir res
  mkdir res/comp
  wget -P ./comp https://code.jquery.com/jquery-3.4.1.js
  wget -P ./comp https://code.angularjs.org/1.7.8/angular.js
  wget -P ./comp https://unpkg.com/mathjs@6.2.3/dist/math.js
  wget -P ./comp https://unpkg.com/react@15.3.2/dist/react.js
  wget -P ./comp https://cdnjs.cloudflare.com/ajax/libs/d3/5.12.0/d3.js
  wget -P ./comp https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.js
  wget -P ./comp https://cdnjs.cloudflare.com/ajax/libs/lodash.js/4.17.15/lodash.js
  wget -P ./comp https://cdnjs.cloudflare.com/ajax/libs/ember.js/2.12.2/ember.prod.js
  wget -P ./comp https://raw.githubusercontent.com/kangax/html-minifier/v4.0.0/dist/htmlminifier.js
  wget -P ./comp https://github.com/photonstorm/phaser/raw/master/dist/phaser.js
fi

rm  res/comp/* 2> /dev/null

TIMEFORMAT=%R
jsminify --version
uglifyjs --version
for f in comp/*; do
  echo $f
  echo jsminify
  time jsminify $f > res/${f%%.js}.min.jsm.js
  echo uglifyjs 
  time uglifyjs $f > res/${f%%.js}.min.ugli.js

  if [ -z "$TIME_ONLY" ]; then
    prettier res/${f%%.js}.min.jsm.js > res/${f%%.js}.pre.jsm.js
    prettier res/${f%%.js}.min.ugli.js > res/${f%%.js}.pre.ugli.js
  fi

  diff res/${f%%.js}.pre.ugli.js res/${f%%.js}.pre.jsm.js &> res/${f%%.js}.diff
done
