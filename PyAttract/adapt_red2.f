      program viewe
      character*80 name1,name2,name3,name4,filnam,b
      character*4 tt(3000),rg(3000),at
      integer re(3000),ka(3000),iai(3000),icb(9000),isi(9000)
      real val(9000),x(9000),y(9000,9000),cha(3000),yy(30,9000),
     1 vval(30)
c  Einlesen des pdb-files und Generierung des pdb-Output files
c   mit dem integer-type code und charges
      call getarg(1,name2)
      read(name2,*) delta
      call getarg(2,name3)
      call getarg(3,name4)
      open(43,file=name3)
      i=1
      jcb=0
      jsi=0
   40 read(43,20,end=35) b
   20 format(a80)
      if(b(:4).eq.'ATOM') then
      ii=3*(i-1)
      read(b,25) at,ka(i),tt(i),rg(i),re(i),
     1           x(ii+1),x(ii+2),x(ii+3)
      write(*,25) at,ka(i),tt(i),rg(i),re(i),
     1           x(ii+1),x(ii+2),x(ii+3)
      if(tt(i).eq.'N   ') then
      jcb=jcb+1
      icb(jcb)=i
      write(*,*)'hello',jcb
      else
      jsi=jsi+1
      isi(jsi)=i
      endif
      i=i+1
      endif 
      goto 40
   35 close(43)
      nall=i-1
      natom=nall-jsi
      write(*,*)'natom',nall,natom,jsi,jcb
      nat3=3*natom
      itest=int(nat3/6)
      irest=nat3-6*itest
      open(45,file=name4)
      do l=1,nat3
      read(45,*) kk,val(l) 
      do i=1,itest
      read(45,*)(y(l,6*(i-1)+j),j=1,6)
      end do
      if(irest.gt.0) read(45,*)(y(l,6*itest+j),j=1,irest)
      end do
      close(45)
      write(*,*)'eigenvalues',(val(i),i=1,20)
      write(*,*)'check norm'
      do k=1,20
      sum=0.0
      do i=1,nat3
      sum=sum+y(k,i)*y(k,i)
      end do
      write(*,'(a10,i5,f10.4)')'sum',k,sum
      enddo
      write(*,*)'check orthogonality'
      do k=1,20
      do j=1,20
      sum=0.0
      do i=1,nat3
      sum=sum+y(j,i)*y(k,i)
      end do
      write(*,'(a10,i5,i5,f10.4)')'sum j,k',j,k,sum
      enddo
      enddo
c star
      kk=0
      do 990 k=nat3-6,nat3-25,-1
      kk=kk+1
      vval(kk)=val(k)
      jb=1
      js=1 
      jn=1
      iflag=0
      do 1000 i=1,nall
      ii=3*(i-1)
      if(icb(jb).eq.i) then
      iflag=jn
      jb=jb+1
      do j=1,3
      yy(kk,ii+j)=y(k,3*(jn-1)+j)
      enddo
      jn=jn+1
      else if(isi(js).eq.i) then
      do j=1,3 
      yy(kk,ii+j)=y(k,3*(iflag-1)+j)
      enddo
      js=js+1
      endif
      write(*,'(2a4,5i4,3f9.4)') 
     1 rg(i),tt(i),i,jb,iflag,jn,js,(yy(kk,ii+j),j=1,3)
 1000 continue
  990 continue
      nall3=3*nall
      itest=int(nall3/6)
      irest=nall3-6*itest
      open(46,file="eignew.out")
      do 1010 i=1,20
      ii=i-1
      write(46,'(i5,f20.10)') i,vval(i)
      do 1020 k=1,itest
      kk=6*(k-1)
      write(46,'(6f15.10)') (yy(i,kk+j),j=1,6)
 1020 continue
      if(irest.gt.0) write(46,'(3f15.10)') (yy(i,nall-3+j),j=1,3)
 1010 continue
      close(46)
      ikk=0
      do in=1,10
      del=1.0/vval(in)*delta
      ikk=ikk+1
      vor=1.0
      write(filnam,177) ikk
  177 format('zwei',i3.3,'.pdb')
      open(55,file=filnam,type='unknown')
      write(55,'(a11)')'Header from'
      do m=-3,3
      do i=1,nall
      ii=3*(i-1)
      xx=x(ii+1)+m*vor*del*yy(in,ii+1)
      xy=x(ii+2)+m*vor*del*yy(in,ii+2)
      xz=x(ii+3)+m*vor*del*yy(in,ii+3)
      write(55,25) at,i,tt(i),rg(i),re(i),xx,xy,xz
      end do
      write(55,'(a3)')'TER'
      write(55,'(a3)')'END'
      end do
      close(55)
      end do
   25 format(a4,i7,2x,a4,a4,i5,4x,3f8.3)
   26 format(a4,i7,2x,a4,a4,i5,4x,3f8.3,i5,f8.3)
 2000 end
