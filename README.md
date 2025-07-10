# HW07

## Pawn 캐릭터 만들기

- 입력 매핑은 IMC를 이용해서 진행하였습니다
  - IA_Move
  - IA_Look

### 이동
- AddActorLocalOffset을 이용해 X, Y축 이동을 합니다.
  - W: x축 +
  - S: x축 -
  - A: y축 -
  - D: y축 +

### 회전
 - AddActorLocalRotation을 이용해 Pawn의 Z축 회전을 합니다.
 - SetRelativeRotation을 이용해 카메라의 Y, Z축 회전을 합니다.

## Drone 만들기
- 기본 매핑은 똑같고 2가지 매핑이 추가되었습니다.
  - IA_Up: 상승
  - IA_Down: 하강
  
### 이동
- Pawn 캐릭터를 이어받고 2가지 입력으로 로직이 추가되었습니다.
  - Space: z축 +
  - Shift: z축 -
    
- 이동 속도
  - 지면: 10(100%) 속도
  - 공중: 5(10*0.5)(50%) 속도
    
- 이동 함수 변경
  - AddActorLocalOffset -> AddActorWorldOffset
  - 이유는 공중에서 이동 중 드론을 약간씩 회전시키는 과정에서 z축의 변화가 있습니다.
  - 공중에서 이동 중 z축 변화가 없게 하기 위해서 변경하였습니다.
  - 
### 회전

- Pawn 회전 그대로 가져왔고 하늘에서 이동 중 살짝 회전하는 것을 추가했습니다.
  
- 이동 중 회전
  - W: 앞으로 기울기
  - S: 뒤로 기울기
  - A: 왼쪽으로 기울기
  - D: 오른쪽으로 기울기
