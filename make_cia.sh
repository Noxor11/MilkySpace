command -v bannertool >/dev/null 2>&1 || {
    echo "bannertool could not be found."
    exit
}

command -v bannertool >/dev/null 2>&1 || {
        echo "makebanner could not be found."
        exit
}

command -v makerom >/dev/null 2>&1 || {
        echo "makerom could not be found."
        exit
}


count=`ls -1 *.3dsx 2>/dev/null | wc -l`
if [ $count == 0 ]
then 
    echo ".3dsx file not found. Did you compile the program with make?"
    exit
fi

game_name=`ls *.3dsx 2>/dev/null | cut -d. -f1`


bannertool makebanner -i banner.png -a romfs/sound/confirm_selection.wav -o $game_name.bnr

makerom -f cia -o $game_name.cia -rsf $game_name.rsf -elf $game_name.elf -exefslogo -icon $game_name.smdh -banner $game_name.bnr

echo "Created $game_name.cia"
