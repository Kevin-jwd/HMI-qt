# 개발 환경 설치 가이드

Qt5 + OpenCV 기반 차량 HMI 프로젝트를 새 PC에서 빌드하기 위한 안내.

## 요구사항

| 항목 | 버전 / 비고 |
|---|---|
| OS | Windows 10 이상 (Linux 도 가능, 아래 별도 항목) |
| 컴파일러 | MinGW-w64 UCRT64 (MSYS2) |
| Qt | 5.15.x (Widgets) |
| OpenCV | 4.x (core, imgproc, videoio, objdetect) |
| 빌드 도구 | qmake + mingw32-make |
| IDE | Qt Creator |

카메라는 2대를 쓴다. 내장(사용자) 캠과 USB 캠.

---

## 1. MSYS2 설치

1. https://www.msys2.org 에서 설치 프로그램을 받아 기본 경로(`C:\msys64`)에 설치한다.
2. 시작 메뉴에서 **MSYS2 UCRT64** 터미널을 연다. (MSYS2 MSYS 나 MINGW64 가 아니라 **UCRT64** 여야 한다)
3. 패키지 목록을 갱신한다. 중간에 터미널이 닫히면 다시 열고 한 번 더 실행한다.

```bash
pacman -Syu
```

## 2. 필요한 패키지 설치

```bash
pacman -S --needed \
  mingw-w64-ucrt-x86_64-toolchain \
  mingw-w64-ucrt-x86_64-qt5-base \
  mingw-w64-ucrt-x86_64-qt5-tools \
  mingw-w64-ucrt-x86_64-make \
  mingw-w64-ucrt-x86_64-opencv \
  mingw-w64-ucrt-x86_64-qt-creator
```

패키지 이름이 맞지 않는다는 오류가 나면 `pacman -Ss qt5-base` 처럼 검색해서 확인한다.
Qt Creator 실행과 킷 설정은 4장에서 이어서 다룬다.

설치 확인:

```bash
g++ --version
qmake --version
ls /ucrt64/include/opencv4/opencv2/core.hpp
ls /ucrt64/lib/libopencv_core*
```

네 명령이 모두 결과를 내면 준비 완료다. 마지막 명령의 파일 이름에 `libopencv_core-4xx.dll.a` 처럼 **버전 숫자가 붙어 있으면**, `hmi.pro` 의 `-lopencv_core` 를 그 이름에 맞게 고쳐야 한다.

## 3. 프로젝트 배치

프로젝트 폴더를 **경로에 한글이 없는 곳**에 둔다. 예: `D:\projects\hmi`

> MSYS2 의 make / moc / uic 는 비ASCII 경로에서 파일을 못 찾는 문제가 잦다.
> `D:\프로젝트\...` 같은 경로는 빌드 도중 원인 모를 실패를 일으킨다.

폴더에 있어야 할 파일:

```
hmi.pro
main.cpp
mainwindow.cpp / mainwindow.h / mainwindow.ui
capturethread.cpp / capturethread.h
displaythread.cpp / displaythread.h
facedetectthread.cpp / facedetectthread.h
framesource.h
```

`build/` 폴더가 딸려 왔다면 **지운다.** 다른 PC 에서 생성된 캐시가 남아 있으면 설정 단계에서 실패한다.

## 4. Qt Creator 설치와 킷 설정

### 4-1. 설치

2장의 패키지 목록에 이미 포함돼 있다. 따로 설치하려면:

```bash
pacman -S mingw-w64-ucrt-x86_64-qt-creator
```

디버깅까지 하려면 gdb 도 필요하다. (toolchain 패키지에 보통 포함돼 있다)

```bash
pacman -S mingw-w64-ucrt-x86_64-gdb
```

> Qt 공식 온라인 설치 프로그램(qt.io)으로 받은 Qt Creator 를 써도 된다.
> 다만 그 경우 Qt 와 컴파일러도 공식 설치본을 쓰는 편이 섞이지 않아 안전하다.
> MSYS2 의 OpenCV 와 조합하려면 경로를 직접 맞춰야 하므로, 이 문서는 MSYS2 기준으로 설명한다.

### 4-2. 실행

**반드시 MSYS2 UCRT64 터미널에서 실행한다.**

```bash
qtcreator &
```

`&` 를 붙이면 터미널을 계속 쓸 수 있다.

바탕화면 바로가기나 탐색기에서 exe 를 직접 실행하면 `PATH` 에
`C:\msys64\ucrt64\bin` 이 없어서, 컴파일러나 DLL 을 못 찾는 문제가 생길 수 있다.
바로가기를 만들고 싶다면 대상에 아래처럼 지정한다.

```
C:\msys64\msys2_shell.cmd -ucrt64 -defterm -no-start -where . -c "qtcreator"
```

### 4-3. 킷 자동 인식 확인

처음 실행하면 MSYS2 의 컴파일러와 Qt 를 스스로 찾는다.
**도구 → 옵션 → Kits** 에서 확인한다.

| 탭 | 있어야 하는 항목 | 경로 |
|---|---|---|
| Compilers | GCC (C), GCC (C++) | `C:\msys64\ucrt64\bin\gcc.exe` / `g++.exe` |
| Debuggers | GDB | `C:\msys64\ucrt64\bin\gdb.exe` |
| Qt Versions | Qt 5.15.x | `C:\msys64\ucrt64\bin\qmake.exe` |
| Kits | 위 셋을 묶은 킷 | 이름 예: `Desktop Qt 5.15.x MinGW-w64 UCRT64 (MSYS2)` |

킷 이름 옆에 **경고(노란 삼각형)나 오류(빨간 X) 아이콘이 없어야 한다.**
아이콘에 마우스를 올리면 무엇이 빠졌는지 알려준다.

### 4-4. 자동 인식이 안 될 때 수동 등록

아래 순서대로 하나씩 추가한다. **순서가 중요하다** — 컴파일러와 Qt 를 먼저
등록해야 킷에서 고를 수 있다.

**① Compilers 탭**

- 추가 → MinGW → C++
  - 이름: `g++ (ucrt64)`
  - 컴파일러 경로: `C:\msys64\ucrt64\bin\g++.exe`
- 추가 → MinGW → C
  - 이름: `gcc (ucrt64)`
  - 컴파일러 경로: `C:\msys64\ucrt64\bin\gcc.exe`

**② Debuggers 탭**

- 추가
  - 이름: `gdb (ucrt64)`
  - 경로: `C:\msys64\ucrt64\bin\gdb.exe`

**③ Qt Versions 탭**

- 추가 → `C:\msys64\ucrt64\bin\qmake.exe` 선택
- 등록 후 버전이 `Qt 5.15.x (ucrt64)` 처럼 표시되는지 확인

**④ Kits 탭**

- 추가 후 아래를 지정한다.

| 항목 | 값 |
|---|---|
| 이름 | `MSYS2 UCRT64 Qt5` |
| Device type | Desktop |
| Compiler C / C++ | 위에서 등록한 gcc / g++ |
| Debugger | 위에서 등록한 gdb |
| Qt version | 위에서 등록한 Qt 5.15.x |
| CMake Tool | (qmake 만 쓸 거면 비워도 됨) |

**⑤ Build & Run → Make 확인**

qmake 빌드는 `mingw32-make.exe` 를 쓴다. 없다면 설치한다.

```bash
pacman -S mingw-w64-ucrt-x86_64-make
```

`C:\msys64\ucrt64\bin\mingw32-make.exe` 가 있는지 확인한다.

### 4-5. 설치 확인용 체크리스트

- [ ] UCRT64 터미널에서 `qtcreator` 실행됨
- [ ] Kits 탭에 킷이 있고 경고 아이콘 없음
- [ ] Compilers 에 gcc / g++ 둘 다 있음
- [ ] Qt Versions 에 qmake 경로가 `ucrt64` 아래로 잡힘
- [ ] `C:\msys64\ucrt64\bin\mingw32-make.exe` 존재
- [ ] 새 프로젝트(File → New → Qt Widgets Application)를 만들어 빌드·실행이 됨

마지막 항목이 가장 확실한 검증이다. 빈 프로젝트가 빌드되면 환경은 정상이고,
이후 문제가 생기면 프로젝트 설정 쪽을 보면 된다.

---

## 5. 프로젝트 열기와 빌드

1. Qt Creator 실행 → 파일 → 파일 또는 프로젝트 열기 → **`hmi.pro`** 선택
2. 킷 선택 화면에서 **Desktop Qt 5.15.x MinGW-w64 UCRT64 (MSYS2)** 체크 → Configure Project
3. 좌측 하단에서 빌드 구성(Debug/Release) 확인 후 빌드 → 실행

킷이 목록에 없으면 도구 → 옵션 → Kits 에서 다음을 확인한다.

- **Compilers** 탭에 `C:\msys64\ucrt64\bin\g++.exe` 가 잡혀 있는지
- **Qt Versions** 탭에 `C:\msys64\ucrt64\bin\qmake.exe` 가 등록돼 있는지
- **Kits** 탭에서 위 둘을 조합한 킷이 있고 경고 아이콘이 없는지

## 6. 실행 전 확인

### 6-1. DLL 경로

빌드는 되는데 실행하면 창이 안 뜨고 바로 종료된다면 DLL 을 못 찾는 것이다.
Qt Creator → 프로젝트 → **실행** → 환경 → `PATH` 에 `C:\msys64\ucrt64\bin` 을 추가한다.

링크는 `lib` 의 임포트 라이브러리로 되지만, 실행 시에는 `bin` 의 DLL 이 필요하다.

### 6-2. 카메라 인덱스

카메라 번호는 PC 마다 다르다. `listcam/` 폴더의 `listcam.pro` 를 따로 열어 빌드하고 실행하면
사용 가능한 인덱스를 확인할 수 있다.

```
list_cameras.exe 4
```

확인한 값을 `mainwindow.h` 에 반영한다.

```cpp
static constexpr int kUserCamSrc = 0;   // 내장(사용자) 캠
static constexpr int kRearCamSrc = 1;   // USB 후방 캠
```

USB 포트를 바꿔 꽂으면 번호가 뒤바뀔 수 있다.

### 6-3. 얼굴 검출용 cascade 파일

`haarcascade_frontalface_default.xml` 이 필요하다. 아래 순서로 자동 탐색하므로,
MSYS2 기본 경로에 OpenCV 를 설치했다면 별도 작업이 없다.

1. 실행 파일과 같은 폴더
2. `C:/msys64/ucrt64/share/opencv4/haarcascades/`
3. 리눅스 기본 경로

"찾지 못했습니다" 메시지가 뜨면 아래 파일을 실행 파일 옆으로 복사한다.

```
C:\msys64\ucrt64\share\opencv4\haarcascades\haarcascade_frontalface_default.xml
```

---

## 7. 자주 겪는 오류

| 증상 | 원인과 해결 |
|---|---|
| `Cannot find file: ...\hmi.pro` | Qt Creator 가 기억한 `.pro` 경로와 실제 파일명이 다름. 파일명을 맞추고 `.pro.user` 를 지운 뒤 다시 연다 |
| `Makefile.Debug:104: debug/hmi_ko_KR.qm Error 2` | `.pro` 의 `TRANSLATIONS` / `CONFIG += lrelease embed_translations` 세 줄을 지운다. 지운 뒤 Run qmake → Rebuild |
| `gui_ui.h: No such file` | include 는 `ui_` 접두사. 폼이 `mainwindow.ui` 면 `#include "ui_mainwindow.h"` |
| `Package opencv4 not found` | `PKGCONFIG` 대신 `hmi.pro` 의 `win32` 블록처럼 경로를 직접 지정한다 |
| `opencv2/core.hpp: No such file` | `INCLUDEPATH` 의 `OPENCV_ROOT` 경로 확인 |
| `undefined reference to cv::...` | 라이브러리 이름 확인. `ls /ucrt64/lib/libopencv_*` 결과와 `-l` 옵션을 맞춘다 |
| 카메라 화면이 조금씩 커짐 | `QLabel` 이 픽스맵 크기를 sizeHint 로 쓰기 때문. `setSizePolicy(Ignored, Ignored)` 로 해결 (이미 적용됨) |
| 탭과 화면이 어긋남 | `navList` 항목 수와 `stackMain` 페이지 수가 달라진 것. Designer 에서 짝을 맞춰 삭제한다 |
| `.pro` 를 고쳤는데 반영 안 됨 | 빌드 → **Run qmake** 후 **Rebuild**. 기존 Makefile 이 남아 있으면 무시된다 |
| `kit ... has configuration issues` | 도구 → 옵션 → Kits 에서 컴파일러·Qt·디버거가 모두 지정됐는지 확인 (4-3, 4-4 참고) |
| `No CMAKE_CXX_COMPILER could be found` | 킷에 컴파일러가 없음. 4-4 의 ① 부터 다시 등록한다 |
| Qt Creator 에서 컴파일러를 못 찾음 | 탐색기에서 실행했을 가능성. UCRT64 터미널에서 `qtcreator &` 로 실행 (4-2 참고) |
| MSYS2 터미널에 붙여넣기 안 됨 | `Ctrl+V` 대신 **마우스 우클릭** 또는 `Shift+Insert` |

---

## 8. Linux 에서 빌드 (참고)

```bash
sudo apt install qtbase5-dev qt5-qmake libopencv-dev
qmake hmi.pro && make -j4
./hmi
```

`hmi.pro` 의 `unix` 블록이 `pkg-config` 로 OpenCV 를 찾으므로 경로 설정이 필요 없다.
카메라 장치는 `ls /dev/video*` 로 확인한다.

---

## 9. 프로젝트 구조

| 파일 | 역할 |
|---|---|
| `main.cpp` | 진입점 |
| `mainwindow.*` | 화면 배선, 카메라 시작/종료 |
| `mainwindow.ui` | Qt Designer 폼 (좌측 탭 + 페이지) |
| `framesource.h` | 프레임 공급자 인터페이스 |
| `capturethread.*` | 카메라에서 최신 프레임만 유지하는 생산자 |
| `displaythread.*` | 프레임을 QImage 로 바꿔 UI 에 전달 |
| `facedetectthread.*` | Haar cascade 얼굴 검출 |
| `listcam/` | 카메라 인덱스 확인용 별도 콘솔 도구 |

카메라 1대당 `CaptureThread` 는 **하나만** 만든다. 여러 화면에 같은 영상을 쓰려면
`DisplayThread` 같은 소비자를 여러 개 붙인다. DirectShow 는 같은 장치를 두 번 열면 실패한다.

---

## 10. 카메라가 없을 때 (동영상 / 사진으로 대체)

카메라 없이도 전체 흐름을 개발·테스트할 수 있다. `MainWindow::createSource()` 가
아래 순서로 입력을 고른다.

1. **환경 변수**로 지정한 파일 또는 폴더
2. 실행 파일 옆의 `sample_user` / `sample_rear` (폴더, `.mp4`, `.avi`, `.jpg`)
3. 둘 다 없으면 실제 카메라

지원하는 형태는 세 가지다.

| 경로 | 동작 |
|---|---|
| 폴더 | 안의 이미지(jpg/png/bmp)를 순환 재생 |
| 이미지 파일 | 정지 화면 |
| 동영상 파일 | 반복 재생 |

### 사용법 A: 실행 파일 옆에 두기

빌드 결과물이 있는 폴더에 `sample_user` 폴더를 만들고 얼굴 사진 여러 장을 넣는다.
그대로 실행하면 카메라 대신 그 사진들이 흘러간다.

```
build-hmi-Desktop-Debug/
  hmi.exe
  sample_user/        <- 얼굴 사진 여러 장
  sample_rear.mp4     <- 후방 영상 대용 (선택)
```

### 사용법 B: 환경 변수로 지정

Qt Creator → 프로젝트 → **실행** → 환경 → Add 로 추가한다.

```
HMI_USER_CAM = D:/samples/myface
HMI_REAR_CAM = D:/samples/road.mp4
```

### 얼굴 학습용 사진 준비

휴대폰으로 찍은 자기 얼굴 사진 15~20장이면 충분하다. 조명과 각도를
조금씩 다르게 찍어 한 폴더에 넣는다. 정면, 약간 좌/우, 밝은 곳/어두운 곳 정도면 된다.

**해상도는 신경 쓰지 않아도 된다.** `FileFrameSource` 가 어떤 크기의 사진이든
카메라 출력과 같은 640x480 으로 맞춰서 내보낸다. 비율은 유지하고 남는 곳은
검은 여백으로 채우므로(레터박스) 얼굴이 찌그러지지 않는다.

다만 촬영할 때 두 가지는 지키는 게 좋다.

- **얼굴이 화면에서 너무 작지 않게.** 640x480 으로 줄인 뒤 얼굴 폭이 최소 40px 은 돼야
  검출된다. 상반신 정도로 찍으면 충분하고, 전신 사진은 피한다.
- **세로 사진이면 여백이 커진다.** 3000x4000 세로 사진은 640x480 안에 넣으면
  가로로 검은 띠가 넓게 생겨 얼굴이 작아진다. 가로로 찍거나 얼굴 위주로 잘라서 넣으면 좋다.

크기 기준을 바꾸고 싶으면 `mainwindow.cpp` 의 `cv::Size(640, 480)` 을 고치면 된다.

### 실제 카메라를 대신하는 다른 방법

- **휴대폰을 웹캠으로**: Windows 11 의 "휴대폰 연결" 기능, 또는 DroidCam / Iriun 같은 앱을
  설치하면 PC 에 일반 웹캠으로 잡힌다. 카메라 인덱스도 정상적으로 부여된다.
- **OBS 가상 카메라**: OBS Studio 에서 동영상 파일을 소스로 넣고 "가상 카메라 시작" 을 누르면
  가상 웹캠 장치가 생긴다. 실제 카메라와 동일하게 동작해서 `CaptureThread` 를 그대로 쓸 수 있다.
