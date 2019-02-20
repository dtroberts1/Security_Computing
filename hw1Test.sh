case $1 in
vigenere.c)
	gcc vigenere.c 
	EXE="./a.out"
	;;
vigenere.cpp)
	g++ vigenere.cpp
	EXE="./a.out"
	;;
vigenere.java)
	javac vigenere.java
	EXE="java vigenere"
	;;
*)
	echo "Invalid source file name"
	exit 1
esac

echo "Case #1"
eval $EXE k1.txt p1.txt >stu1Output.txt
read -n 1 -p "Enter to continue:" mainmenuinput
diff stu1Output.txt case1Base.txt
echo "Case #1 - complete"

echo "Case #2"
eval $EXE k2.txt p2.txt >stu2Output.txt
read -n 1 -p "Enter to continue:" mainmenuinput
diff stu2Output.txt case2Base.txt
echo "Case #2 - complete"

echo "Case #3"
eval $EXE k3.txt p3.txt >stu3Output.txt
read -n 1 -p "Enter to continue:" mainmenuinput
diff stu3Output.txt case3Base.txt
echo "Case #3 - complete"

echo "Case #4"
eval $EXE k4.txt p4.txt >stu4Output.txt
read -n 1 -p "Enter to continue:" mainmenuinput
diff stu4Output.txt case4Base.txt
echo "Case #4 - complete"
