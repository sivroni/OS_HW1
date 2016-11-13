mkdir 305560336
cd 305560336

mkdir temp
cd temp
echo ' Maayan' > Maayan.txt
echo 'Sivroni' > Sivroni.txt
echo 'sivroni' > sivroni.txt

cp Maayan.txt ../Sivroni.txt
cp Sivroni.txt ../Maayan.txt

rm Maayan.txt Sivroni.txt

mv sivroni.txt ..
cd ..
rmdir temp

echo The following list contains all files in directory:
ls -al
echo The content of "Maayan.txt" is:
cat Maayan.txt
echo The content of "Sivroni.txt" is:
cat Sivroni.txt
echo The content of "sivroni.txt" is:
cat sivroni.txt