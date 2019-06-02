//´ã´ç : ½ÂÃ¶

void CheckInteractState(float fTime);
void ChangeInteractState(int iState, float fTime);
void SetInteractRotationVector(const Vector3& vec);
void StateHideInBed(float fTime);
void StateHidingBed(float fTime);
void StateGetOutBed(float fTime);
void HidingMotionEnd(float fTime);

void Interact_With_Locker(class CLocker *pLocker, float fTime);
void Interact_Exit_Locker(class CLocker *pLocker, float fTime);

void Interact_With_Door(class CDoor *pDoor, float fTime);
void Interact_Exit_Door(class CDoor *pDoor, float fTime);

void Interact_With_Bed(class CBed *pBed, float fTime);
void Interact_Exit_Bed(class CBed *pBed, float fTime);

void Geometry_Push(CCollider *pSrc, CCollider *pDest, float fTime);