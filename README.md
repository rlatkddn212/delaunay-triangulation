# delaunay-triangulation
delaunay-triangulation c++ mfc
  
들로네 삼각화 알고리즘을 구현한 코드입니다.  
visual studio 2017 mfc로 개발 완료되었습니다.  
이 코드는 computational Geometry Algorithms and Applications 서적에 나오는 알고리즘을 구현했습니다.  
책에 내용 처럼 random으로 point를 증가시키고 있진 않습니다. 필요하다면 random한 순서로 셔플하는 알고리즘을 추가하시면됩니다.  

알고리즘의 성능은 최악의 경우 O(n^2), 평균적인 경우 O(nlogn)에 동작합니다.  
point 입력시 중복되는 point가 없도록 처리해주세요. (이 프로그램은 알고리즘 동작전 동일한 point를 제거하고 있습니다.)


![delaunay](https://github.com/rlatkddn212/delaunay-triangulation/blob/master/Delaunay.png)


  
참고 자료  
http://www.secmem.org/blog/2019/01/11/Deluanay_Triangulation/  
https://kipl.tistory.com/16  
computational Geometry Algorithms and Applications 9장
