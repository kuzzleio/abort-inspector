#! /bin/bash

function run {
  LOGFILE=$1.log
  echo "$@"
  "$@" >$LOGFILE 2>&1

  if [ $? -ne 0 ]; then
    cat $LOGFILE
    rm $LOGFILE
    echo "=== $1 failed. Exiting."
    exit 1
  fi

  rm $LOGFILE
}

CWD=$(cd $(dirname $0) && pwd)

cd $CWD

cd $CWD/libunwind

# run ./autogen.sh --disable-tests
run ./configure --prefix=$CWD --disable-tests
run make clean
run make
run make install prefix=$CWD

cd $CWD

npm run configure
npm run build
