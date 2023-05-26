if [[ $# -eq 1 ]]
then
    if [[ $1 = build ]]
    then
        export CPATH+="$PWD/thirdParty/json/include"
        echo "CPATH=" $CPATH;
        cd Client;
        make;
        cd ..;
    elif [[ $1 = clean ]]
    then
        cd Client;
        make clean;
    fi
elif [[ $# -eq 0 ]]
then
    echo $LD_LIBRARY_PATH;
    export LD_LIBRARY_PATH=$PWD/thirdParty/restbed/build;
    echo "LD_LIBRARY_PATH= " $LD_LIBRARY_PATH;
    ./Client/Bin/clientRestApi.bin  
fi
