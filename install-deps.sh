git clone http://github.com/ptools/ptools_dep

if [ $? -ne 0 ]
  then 
    echo "the git command failed, trying with wget"
    wget https://github.com/ptools/ptools_dep/tarball/master -O - | tar xz
    
    if [ $? -ne 0 ]
      then 
         echo wget failed, trying curl
         curl https://codeload.github.com/ptools/ptools_dep/legacy.tar.gz/master | tar xz  #this url is different since github adds a redirection
    fi  
    
    if [ $? -ne 0 ]
       then
       echo "neither git, wget nor curl succeeded downloading ptools dependencies. Please install one of those tools or make sure
             that your internet connection is OK ( http proxy?)"
       exit 1
    
    fi
    #rename the newly created directory:
    mv ptools-ptools_dep* ptools_dep
  
fi

cd ptools_dep/libf2c2-20090411 && make -f makefile.u CFLAGS="-ansi -g -O2 -fomit-frame-pointer -D_GNU_SOURCE  -fPIC -DNON_UNIX_STDIO -Df2c"
make -f makefile.u CFLAGS="-ansi -g -O2 -fomit-frame-pointer -D_GNU_SOURCE  -fPIC -DNON_UNIX_STDIO -Df2c" libf2c.a
if [ $? -ne 0 ]
   then
     echo "compilation failed, please modify compilation flags"
     exit 1
   fi

cd ..
mkdir f2c
cp libf2c2-20090411/f2c.h f2c
cp libf2c2-20090411/libf2c.a f2c


if [ $? -eq 0 ]
  then
     echo "the last make command succeeded, you can now try to run 'python setup.py build'"
fi
