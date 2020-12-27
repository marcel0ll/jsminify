for t in $(ls js); do 
  jsminify -d js/$t &> tmp/$t
done

