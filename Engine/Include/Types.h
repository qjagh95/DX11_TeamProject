#pragma once

#include <ostream>

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

	//////////////////////////////// VERTEX INPUT LAYOUT INFO /////////////////////////////

	// USE AT RESOURCES MANAGER CLASS

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

	struct PUN_DLL Vertex3DColor
	{
		Vector3		vPos;
		Vector2		vUV;
		Vector4		vColor;

		Vertex3DColor()
		{
			vColor = Vector4::White;
		}
	};


	/////////////////////////////// VERTEX INPUT LAYOUT INFO END //////////////////////////


	////////////////////////////////// CONSTANT BUFFER ////////////////////////////////////

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
		Matrix	matWLP;
		Matrix	matInvWVP;
		Matrix  matInvProj;
		Matrix	matVP;
		Matrix	matInvVP;
		Matrix	matLP;
		Vector3	vPivot;
		int		iShadowEnable;
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
		int		iDeffered;
		int		iFocus;
		int		iDecalEnable;
		int		i3DAnimation;

		Vector2 vEmpty;

		_tagComponentCBuffer() :
			iTextureAnimation2D(0),
			iDeffered(1),
			iFocus(0),
			iDecalEnable(1)
		{
		}
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
		int		iRimLight;
	}LightInfo, *PLightInfo;

	typedef struct PUN_DLL _tagRimLightInfo
	{
		Vector3 vRimColor;
		float	fRimPower;
	}RimLightInfo, *PRimLightInfo;

	struct PUN_DLL PublicCBuffer
	{
		float	DeltaTime;
		float	PlusedDeltaTime;
		int		isDeferred;
		int		isWireFrame;
		Vector2 vNearFar;
		Vector2 ViewPortSize;
		int		iSSAOEnable;
		Vector3 vEmpty;

		PublicCBuffer()	:
			isDeferred(1),
			iSSAOEnable(1)
		{
		}
	};

	typedef struct PUN_DLL _tagLandScapeCBuffer
	{
		float	fDetailLevel;
		int		iSplatCount;
		Vector2	vEmpty;
	}LandScapeCBuffer, *PLandScapeCBuffer;

	typedef struct _tagHDRFirstPassCBuffer
	{
		int		iWidth;
		int		iHeight;
		int		iTotalPixels;
		int		iGroupSize;
	}HDR1stPassCB, *PHDR1stPassCB;

	typedef struct _tagHDRSecondPassCB
	{
		float	fMiddleGrey;
		float	fLumWhite;
		Vector2	vEmpty;
	}HDR2ndPassCB, *PHDR2ndPassCB;

	typedef struct _tagAdaptationCB
	{
		float	fAdaptation;
		Vector3 vEmpty;
	}AdaptationCB, *PAdaptationCB;

	typedef struct _tagBloomThresholdCB
	{
		float	fBloomThreshold;
		Vector3 vEmpty;
	}BloomThresholdCB, *PBloomThresholdCB;

	typedef struct _tagBloomCB
	{
		float	fBloomScale;
		Vector3 vEmpty;
	}BloomCB, *PBloomCB;

	typedef struct PUN_DLL _tagBlurCBuffer
	{
		int			iSize;
		int			iDepthOfField;
		float		fCameraFar;
		float		fEmpty;

		_tagBlurCBuffer() :
			iSize(0),
			iDepthOfField(0),
			fCameraFar(1000.0f)
		{
		}

		~_tagBlurCBuffer()
		{
		}
	}BlurCBuffer, *PBlurCBuffer;

	typedef struct PUN_DLL _tagFinalPassCBuffer
	{
		int			iHDR;
		int			iBlur;
		int			iMotionBlur;
		int			iDepthFog;
		int			iBloom;
		int			iAdaptation;
		Vector2		vEmpty;

		_tagFinalPassCBuffer() :
			iHDR(0),
			iBlur(0),
			iMotionBlur(0),
			iDepthFog(0),
			iBloom(0),
			iAdaptation(0)
		{
		}

	}FinalPassCB, *PFinalPassCB;

	typedef struct PUN_DLL _tagParticleCBuffer
	{
		Vector3		vCenter;
		float		fSizeX;
		Vector3		vAxisX;
		float		fSizeY;
		Vector3		vAxisY;
		float		fEmpty;
	}ParticleCBuffer, *PParticleCBuffer;

	typedef struct PUN_DLL _tagFogCBuffer
	{
		Vector4 vFogColor;
		float	fDensity;
		Vector3 vEmpty;

		_tagFogCBuffer()
		{
			vFogColor = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
	}FogCBuffer, *PFogCBuffer;

	typedef struct PUN_DLL _tagStarLightScope
	{
		int isStarLightScope;
		int isShake;
		int isSign;
		float fEmpty;
		Vector2 vRandomPos;
		Vector2 vEmpty;
		_tagStarLightScope() :
			isStarLightScope(0),
			isShake(0),
			isSign(0)
		{
		}
	}StarLightScopeCB, *PStarLightScopeCB;

	typedef struct PUN_DLL _tagDepthFogCBuffer
	{
		Vector4 vFogColor;
		float fStartDepth;
		float fEndDepth;
		Vector2 vEmpty;
	}DepthFogCBuffer, *PDepthFogCBuffer;

	///////////////////////////////// CONSTANT BUFFER END /////////////////////////////////


	///////////////////////////////// COLLIDER INFO ///////////////////////////////////////
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

	typedef struct PUN_DLL _tagAABBInfo
	{
		Vector3	vCenter;
		Vector3	vLength;
	}AABBInfo, *PAABBInfo;

	typedef struct PUN_DLL _tagRayInfo
	{
		Vector3	vPos;
		Vector3	vDir;
		float	fDist;
	}RayInfo, *PRayInfo;

	////////////////////////////////////// COLLIDER INFO END //////////////////////////////

	struct PUN_DLL TimeInfo
	{
		clock_t Start;
		clock_t End;
	};

	struct PUN_DLL FileStream
	{
		std::ofstream Input;
		std::ofstream Update;
		std::ofstream LateUpdate;
		std::ofstream Collsion;
		std::ofstream Render;

		~FileStream() { Input.close(); Update.close(); LateUpdate.close(); Collsion.close(); Render.close(); }
	};
}