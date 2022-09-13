command -v bannertool >/dev/null 2>&1 || {
    echo "bannertool could not be found."
    exit
}

command -v bannertool >/dev/null 2>&1 || {
        echo "makebanner not be found."
        exit
}

if [[ `ls *.3dsx 2>/dev/null` != "SpaceGame.3dsx" ]]
then
    echo ".3dsx file not found. Did you compile the program with make?"
    exit
fi


bannertool makebanner -i banner.png -a romfs/sound/confirm_selection.wav -o SpaceGame.bnr

makerom -f cia -o SpaceGame.cia -rsf SpaceGame.rsf -elf SpaceGame.elf -exefslogo -icon SpaceGame.smdh -banner SpaceGame.bnr

echo "Created SpaceGame.cia"
