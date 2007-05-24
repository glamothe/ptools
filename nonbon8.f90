
subroutine nonbon8(recCoords, r ,ligCoords,l, rectypes, ligtypes, cha_r, cha_l, nonr, nonl, nonp , ac, rc, LJ, coulomb )
    implicit none

    integer  :: r, l ! ligand and receptor sizes
    double precision  :: ligCoords(3*l)
    double precision  :: recCoords(3*r)
    integer           :: rectypes(r)  !atom types for receptor atoms
    integer           :: ligtypes(l)  !atoms types for ligand atoms
    double precision  ac(40,40), rc(40,40)
    double precision  cha_r(r)
    double precision  cha_l(l)
 

    integer  nonp  !size of the pairlist
    integer  nonr(nonp), nonl(nonp) !pairlist for ligand l and receptor r


    integer  ik,i,j , ii, jj,k

    double precision  dx(3)
    double precision  r2, rr23, rr2, LJ, coulomb
    double precision  alen, rlen, rep, xnull, charge

    !write(*,*) 'routine fortran. nonp= ', nonp
    !write(*,*) 'size of receptor: ', r, l


    xnull=0.0d0
    LJ=0.0d0
    coulomb=0.0d0


!c-----intialization---------------------------------------
!       do 5 i=1,natom
!       do 6 j=1,nlig
!       et(i,j)=xnull
!       vlj(i,j)=xnull
!       do 7 k=1,3
!       fdb(i,j,k)=xnull
!  7    continue
!  6    continue
!  5    continue
!c--------------------------------------------------------



!c  goes through all pairs in the nonbonded list (max number of pairs: nonp)
    do 100 ik=1,nonp
       ! write(*,*) "ik: ", ik
        !selects partner in receptor
        i=nonr(ik)+1
       ! write(*,*) "i: ",i
        !selects partner in ligand
        j=nonl(ik)+1
        !write(*,*) 'i,j: ',i,j
        ii=3*(i-1)
        jj=3*(j-1)
        !selects LJ parameters
        alen=ac(ligtypes(j)+1,rectypes(i)+1)
        rlen=rc(ligtypes(j)+1,rectypes(i)+1)
        !write(*,*) "(F90) atcat R: ",rectypes(i) , "   atcat L: ", ligtypes(j)
        !write(*,*) "(F90) alen, rlen: ", alen, rlen 
        charge=cha_r(i)*cha_l(j)*(332.053986/20.0)
      r2=xnull
!c     calculate distances (squared)
      do 120 k=1,3
      dx(k)=ligCoords(jj+k)- recCoords(ii+k)
      r2=r2+dx(k)**2
  120 continue
      !write(*,*) "(f90) dx: ",dx(1), dx(2), dx(3)

      if(r2.lt.0.001d0) r2=0.001d0
!c     calculates reverse of the distance(squared)
      rr2=1.0d0/r2
      do 125 k=1,3
      dx(k)=rr2*dx(k)
  125 continue
!c     if iab=1 : calculates the forces
      if(abs(charge).gt.xnull) then
      !write(*,*) "charge non nulle"
      coulomb=coulomb+charge*rr2
     ! if(iab.eq.1) then
    !  do 130 k=1,3
   !   fdb(i,j,k)=2.0d0*et(i,j)*dx(k)
  !130 continue
     ! endif
      endif
!c     here the Lennard Jones energy and forces are calculated
      rr23=rr2**3
      rep=rlen*rr2
      LJ=LJ+(rep-alen)*rr23
      !if(iab.eq.1) then
      !fb=6.0D0*vlj(i,j)+2.0D0*(rep*rr23)
      !do 135 k=1,3
      !fdb(i,j,k)=fdb(i,j,k)+fb*dx(k)
  !135 continue
   !   endif

  100 continue
      !write(*,*) "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa"
      !write(*,*) 'LJ: ', LJ, '   coulomb: ', coulomb
      return
      end



