GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

correct=1
for t in $(ls js); do 
  curt=1
  jsminify js/$t &> tmp/$t
  diff min/$t tmp/$t &> /dev/null || curt=0

  if [ $curt -eq 1 ]; then
    echo -e "$GREEN Test $NC$t$GREEN passed"
  else
    echo -e "$RED Test $NC$t$RED failed"
    correct=0
  fi
done

echo
if [ $correct -eq 1 ]; then
  echo -e "$GREEN All tests passed$NC"
else
  echo -e "$RED Tests failed!$NC"
fi

