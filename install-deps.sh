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

cd ptools_dep/f2c && make
if [ $? -eq 0 ]
  then
     echo "the last make command succeeded, you can now try to run 'python setup.py build'"
fi