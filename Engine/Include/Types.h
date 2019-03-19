#pragma once

#define PUN_BEGIN	namespace PUN{
#define PUN_END		}
#define PUN_USING	using namespace PUN;

#ifdef PUN_EXPORT
#define PUN_DLL __declspec(dllexport) // 메모리 정렬도 해준다
#else
#define PUN_DLL __declspec(dllimport)
#endif // PUN_EXPORT

#include "Flag.h"
#include "Macro.h"

namespace PUN
{
	typedef struct PUN_DLL _tagResolution
	{
		unsigned int	iWidth;
		unsigned int	iHeight;

		_tagResolution() :
			iWidth(0),
			iHeight(0)
		{
		}

		_tagResolution(const _tagResolution& rs)
		{
			*this = rs;
		}

		_tagResolution(unsigned int iW, unsigned int iH) :
			iWidth(iW),
			iHeight(iH)
		{
		}
	}Resolution, *PResolution;

	// Vertex Color
	typedef struct _tagVertexColor
	{
		Vector3	vPos;
		Vector4	vColor;

		_tagVertexColor()
		{
		}

		_tagVertexColor(const _tagVertexColor& v)
		{
			*this = v;
		}

		_tagVertexColor(const Vector3& vP, const Vector4& vC) :
			vPos(vP),
			vColor(vC)
		{
		}
	}VertexColor, *PVertexColor;

	// Vertex Normal Color
	typedef struct _tagVertexNormalColor
	{
		Vector3	vPos;
		Vector3	vNormal;
		Vector4	vColor;

		_tagVertexNormalColor()
		{
		}

		_tagVertexNormalColor(const _tagVertexNormalColor& v)
		{
			*this = v;
		}

		_tagVertexNormalColor(const Vector3& vP, const Vector3& vN, const Vector4& vC) :
			vPos(vP),
			vNormal(vN),
			vColor(vC)
		{
		}
	}VertexNormalColor, *PVertexNormalColor;

	// Vertex Tex
	typedef struct _tagVertexTex
	{
		Vector3	vPos;
		Vector2	vUV;

		_tagVertexTex()
		{
		}

		_tagVertexTex(const _tagVertexTex& v)
		{
			*this = v;
		}

		_tagVertexTex(const Vector3& vP, const Vector2& vT) :
			vPos(vP),
			vUV(vT)
		{
		}
	}VertexTex, *PVertexTex;

	typedef struct PUN_DLL _tagCBuffer
	{
		ID3D11Buffer*	pBuffer;
		int				iSize;
		int				iShaderType;
		int				iRegister;
	}CBuffer, *PCBuffer;

	typedef struct PUN_DLL _tagTransformCBuffer
	{
		Matrix	matWorld;
		Matrix	matView;
		Matrix	matProj;
		Matrix	matWV;
		Matrix	matWVP;
		Matrix  matInvProj;
		Vector3	vPivot;
		float	fEmpty;
		Vector3	vLength;
		float	fEmpty1;
	}TransformCBuffer, *PTransformCBuffer;

	typedef struct PUN_DLL _tagMaterial
	{
		Vector4	vDiffuse;
		Vector4	vAmbient;
		Vector4	vSpecular;
		Vector4	vEmissive;

		_tagMaterial() :
			vDiffuse(Vector4::White),
			vAmbient(Vector4(0.2f, 0.2f, 0.2f, 1.f)),
			vSpecular(Vector4::White)
		{
			vSpecular.w = 3.2f;
		}
	}Material, *PMaterial;

	typedef struct PUN_DLL _tagComponentCBuffer
	{
		int		iTextureAnimation2D;
		int		iAnimationType;
		Vector2	fEmpty;
	}ComponentCBuffer, *PComponentCBuffer;

	typedef struct PUN_DLL _tagClip2DFrame
	{
		Vector2	vLT;
		Vector2	vRB;
	}Clip2DFrame, *PClip2DFrame;

	typedef struct PUN_DLL _tagAnimationClip2D
	{
		ANIMATION2D_TYPE	eType;
		ANIMATION_OPTION	eOption;
		string				strName;
		class CTexture*		pTexture;
		int					iTexRegister;
		float				fTextureWidth;
		float				fTextureHeight;
		vector<Clip2DFrame>	vecFrame;
		int					iFrame;
		float				fPlayTime;
		float				fPlayLimitTime;
	}AnimationClip2D, *PAnimationClip2D;

	typedef struct PUN_DLL _tagAnimation2DCBuffer
	{
		Vector2	vUVLT;
		Vector2	vUVRB;
		int		iFrame;
		Vector3	vEmpty;
	}Animation2DCBuffer, *PAnimation2DCBuffer;

	typedef struct PUN_DLL _tagRendererCBuffer
	{
		void*	pBuffer;
		string	strName;
		int		iSize;
	}RendererCBuffer, *PRendererCBuffer;

	typedef struct PUN_DLL _tagBoxInfo
	{
		Vector3	vMin;
		Vector3	vMax;
		Vector3	vLength;
	}BoxInfo, *PBoxInfo;

	typedef struct PUN_DLL _tagOBB2DInfo
	{
		Vector3	vCenter;
		Vector3	vAxis[2];
		float	fLength[2];
	}OBB2DInfo, *POBB2DInfo;

	typedef struct PUN_DLL _tagPixel24
	{
		unsigned char	r;
		unsigned char	g;
		unsigned char	b;
	}Pixel24, *PPixel24;

	typedef struct PUN_DLL _tagPixel32
	{
		unsigned char	r;
		unsigned char	g;
		unsigned char	b;
		unsigned char	a;
	}Pixel32, *PPixel32;

	typedef struct PUN_DLL _tagPixelInfo
	{
		BoxInfo			tBox;
		Pixel24*		pPixel;
		int				iWidth;
		int				iHeight;
		Pixel24			tYasuoPixel;
	}PixelInfo, *PPixelInfo;

	typedef struct PUN_DLL _tagButtonCBuffer
	{
		Vector4		vColor;
	}ButtonCBuffer, *PButtonCBuffer;


	typedef struct PUN_DLL _tagBarCBuffer
	{
		float	fPercent;
		int		iMoveDir;
		Vector2	vEmpty;
		Vector4	vLight;
	}BarCBuffer, *PBarCBuffer;

	typedef struct PUN_DLL _tagLightInfo
	{
		Vector4	vDif;
		Vector4	vAmb;
		Vector4	vSpc;
		int		iLightType;
		Vector3	vPos;
		Vector3	vDir;
		float	fRange;
		float	fInAngle;
		float	fOutAngle;
		float   fFallOff;
		float	vEmpty;
	}LightInfo, *PLightInfo;

	typedef struct PUN_DLL _tagVertexNormalTex
	{
		Vector3		vPos;
		Vector3		vNormal;
		Vector2		vUV;
	}VertexNormalTex, *PVertexNormalTex;

	typedef struct PUN_DLL _tagVertex3D
	{
		Vector3		vPos;
		Vector3		vNormal;
		Vector2		vUV;
		Vector3		vTangent;
		Vector3		vBinormal;
		Vector4		vWeight;
		Vector4		vIndex;
	}Vertex3D, *PVertex3D;

	struct PUN_DLL PublicCBuffer
	{
		float DeltaTime;
		float PlusedDeltaTime;
		int isDeferred;
		int isWireFrame;
		int Empty1234;
		Vector2 ViewPortSize;
		float Empty;
	};

	typedef struct PUN_DLL _tagLandScapeCBuffer
	{
		float	fDetailLevel;
		int		iSplatCount;
		Vector2	vEmpty;
	}LandScapeCBuffer, *PLandScapeCBuffer;

	struct PUN_DLL TimeInfo
	{
		clock_t Start;
		clock_t End;
	};

	struct PUN_DLL FileStream
	{
		ofstream Input;
		ofstream Update;
		ofstream LateUpdate;
		ofstream Collsion;
		ofstream Render;

		~FileStream() { Input.close(); Update.close(); LateUpdate.close(); Collsion.close(); Render.close(); }
	};

	typedef struct PUN_DLL _tagSphereInfo
	{
		Vector3	vCenter;
		float	fRadius;

		_tagSphereInfo() :
			fRadius(1.f)
		{
		}
	}SphereInfo, *PSphereInfo;

	typedef struct PUN_DLL _tagOBB3DInfo
	{
		Vector3	vCenter;
		Vector3	vAxis[3];
		Vector3	vLength;
	}OBB3DInfo, *POBB3DInfo;

	typedef struct _tagDownScaleCBuffer
	{
		int		iWidth;
		int		iHeight;
		int		iTotalPixels;
		int		iGroupSize;
	}DownScaleCB, *PDownScaleCB;

	typedef struct _tagFinalPassCB
	{
		float	fMiddleGrey;
		float	fLumWhite;
		Vector2	vEmpty;
	}FinalPassCB, *PFinalPassCB;

	typedef struct _tagAdaptationCB
	{
		Vector4	vAdaptation;
	}AdaptationCB, *PAdaptationCB;
}