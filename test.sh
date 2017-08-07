./tft r
./tft +1 日本語
./tft +2 ｶﾀﾅｶ
./tft +3 ABCDEFG
./tft +4 1234567
sudo ./tft s
echo -n "push Enter"
read input

./tft B0 BLACK
./tft F1 WHITE
./tft F2 WHITE
./tft F3 WHITE
./tft F4 WHITE
sudo ./tft s
echo -n "push Enter"
read input

./tft F1 WHITE
./tft F2 YELLOW
./tft F3 CYAN
./tft F4 BLUE
sudo ./tft s
echo -n "push Enter"
read input

./tft R90
sudo ./tft s
echo -n "push Enter"
read input

./tft R180
sudo ./tft s
echo -n "push Enter"
read input

./tft R270
sudo ./tft s
echo -n "push Enter"
read input
