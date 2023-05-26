if [[ $# -eq 1 ]]
then
    if [[ $1 = build ]]
    then
        export CPATH+="$PWD/thirdParty/json/include"
        echo "CPATH=" $CPATH;
        cd Server;
        make;
        cd ..;
        #./Server/Bin/serverRestApi.bin
    elif [[ $1 = clean ]]
    then
        cd Server;
        make clean;
    fi
elif [[ $# -eq 0 ]]
then
    echo $LD_LIBRARY_PATH;
    export LD_LIBRARY_PATH=$PWD/thirdParty/restbed/build;
    echo "LD_LIBRARY_PATH= " $LD_LIBRARY_PATH;
    ./Server/Bin/serverRestApi.bin 
fi


