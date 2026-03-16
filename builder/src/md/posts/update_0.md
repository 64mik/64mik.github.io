---
title:1.1.0 업데이트
date:2026-03-16
lore:리펙토링 및 일부 기능 개선
tags:none
category:update
isPost:true
---

<= title
v 1.1.0 업데이트
=>
<= box
    <= title
        [ 변경사항 ] : 리펙토링
    =>
    <= hr
    =>
    <= bold
        (metaData)
    =>
    - pageData라는 구조체 추가 및 정상 작동
    - 버전 및 각종 정보 기본값 부여함
    - pushMeta, getMeta 작성
    

    <= bold
        (builder)
    =>
    - builder 로 만들어지는 파일 경로 구조체로 직관적으로 변경
    - builder.cpp 전반적으로 리펙토링
    - 캐싱 기능을 하는 hashing()이 기존 1바이트씩 읽는 get 함수를 쓰는 것이 비효율이라 판단 -> 버퍼 사용으로 개선
    - makePost가 너무 많은 것을 작업한다 판단해 구조 변경 및 일부 기능 축출


    <= bold
        (main)
    =>
    - 기존 선택지인 mdToHtml 와 publish post의 코드가 상당수 겹침을 인지, processing으로 묶음
    - 람다를 활용해 재사용성 확보
    - 기존의 스트링 입력 방식을 정수 입력 및 스위치 문으로 변경
    - index.html을 제외한 나머지 모든 html을 pages 폴더에 옮김
    - post 와 page 분리 처리 기능 추가


    <= bold
        (기타)
    =>
    - 64mik.icon 변경
    - 프로젝트 폴더의 위치 일부 변경
    - 프로젝트 폴더 이름 일부 변경
    - 기존에 존재하던 혹은 나중에 재사용 가능할 파일은 lib에 분리
    - 캐싱 기능을 적출해 lib에 옮김
    - conf 파일 내용 일부 수정 (path 등)
=>

<= box
    <= title
        [ 추후 수정 ]
    =>
    <= hr
    =>
    - 내부적으로 error 코드 작성방식 통일 예정 ([xxxx] error: text 방식)
    - 글 목록 / 최신 글 출력
    - 로그 파일
=>