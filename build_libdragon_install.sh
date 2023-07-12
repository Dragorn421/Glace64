set -e

if [ -z ${N64_GCCPREFIX+x} ]  # if N64_GCCPREFIX is not set
then
    if [ -z ${N64_INST+x} ]  # if N64_INST is not set
    then
        echo 'Neither N64_GCCPREFIX nor N64_INST is set, cannot compile libdragon.'
        exit 1
    else
        N64_GCCPREFIX=$N64_INST
    fi
fi

mkdir -p libdragon_install

export N64_GCCPREFIX
export N64_INST=$(realpath ./libdragon_install)

echo N64_GCCPREFIX=$N64_GCCPREFIX
echo N64_INST=$N64_INST

echo 'Building libdragon...'
make -C libdragon libdragon "$@"
echo 'Building libdragon tools...'
make -C libdragon tools "$@"
echo 'Installing libdragon and libdragon tools...'
make -C libdragon install tools-install "$@"
