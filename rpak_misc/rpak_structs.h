#pragma once
#include <cstdint>

// vectors
struct Vector2
{
	float x, y;

	Vector2(float x, float y) {
		this->x;
		this->y;
	}

	Vector2() {};
};

struct Vector3
{
	float x, y, z;

	Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3() {};
};

// generic rpak
struct RPakPtr
{
    uint32_t Index;
    uint32_t Offset;
};

// aseq headers
struct ASeqHeaderV7
{
	RPakPtr data; // pointer to raw rseq.
	RPakPtr szname; // pointer to debug name, placed before raw rseq normally.

	// this can point to a group of guids and not one singular one.
	RPakPtr modelhash; // hehe haha I love unverified assumptions
	uint32_t modelcount; // to be verified on v7

    uint32_t reserved; // haha surely because this is used later?

	// this can point to a group of guids and not one singular one.
	RPakPtr stgshash;
	uint32_t stgscount;

    uint32_t reserved1; // hehe assuming like above :DDD
};

struct ASeqHeaderV71
{
    RPakPtr data; // pointer to raw rseq.
    RPakPtr szname; // pointer to debug name, placed before raw rseq normally.

	// this can point to a group of guids and not one singular one.
    RPakPtr modelhash; // hehe haha I love unverified assumptions
    uint32_t modelcount; // to be verified on v7

	// size of the external data.
	uint32_t externaldatasize;

	// this can point to a group of guids and not one singular one.
    RPakPtr stgshash;
    uint32_t stgscount;

    uint32_t reserved1; // hehe assuming like above :DDD

	// data that is stored outside of the raw rseq.
    RPakPtr externaldata;
};

struct ASeqHeaderV10
{
	RPakPtr data; // pointer to raw rseq.
	RPakPtr szname; // pointer to debug name, placed before raw rseq normally.

    uint64_t Unknown; // possible pointer, guid, or reserved space.

	// counts for mdl_ and stgs assets, normally just one but can be multiples.
    uint16_t modelcount;
    uint16_t stgscount;

	// size of the external data.
    uint32_t externaldatasize;

	// these can all point to a group of guids and not one singular one.
    RPakPtr modelhash;
	RPakPtr efcthash;
    RPakPtr stgshash;

	// data that is stored outside of the raw rseq.
    RPakPtr externaldata;
};

// rpak matl headers unk section
struct UnknownMaterialSectionV12
{
	// not sure how these work but 0xF0 -> 0x00 toggles them off and vice versa.
	// they seem to affect various rendering filters, said filters might actually be the used shaders.
	// the duplicate one is likely for streamed textures.
	uint32_t unkrenderlighting = 0;
	uint32_t unkrenderaliasing = 0;
	uint32_t unkrenderDoF = 0;
	uint32_t unkrenderunknown = 0;

	uint32_t unkflags; // this changes sometimes.
	uint16_t visibilityflags; // different render settings, such as opacity and transparency.
	uint16_t faceflags; // how the face is drawn, culling, wireframe, etc.

	uint64_t reserved;
};

struct UnknownMaterialSectionV15
{
	// nulling these bytes makes the material stop drawing entirely
	uint32_t unkrenderunknown[8];

	// for more details see the 'UnknownMaterialSectionV12' struct.
	uint32_t unkflags; // this changes sometimes.
	uint16_t visibilityflags; // different render settings, such as opacity and transparency.
	uint16_t faceflags; // how the face is drawn, culling, wireframe, etc.

	uint64_t reserved;
};

/*VisibilityFlags
0x0000 unknown
0x0001 inverted ignorez
0x0002 required when ignorez is enabled, why?
0x0004 unknown but used in most opaque materials, not required.
0x0008
0x0010 seems to toggle transparency, will draw opaque if inverted ignorez is enabled
0x0017 used for most normal materials.
0x0007 used for glass which makes me think 0x0010 is for translucency.
0x0013 is vaild and looks like a normal opaque material.  */

/*FlagDrawingFlags Flags
0x0000 culling this is the same as 0x0002??? maybe the default?
0x0001 wireframe
0x0002 normal texture drawing aka culling (front side and backside drawn).
0x0004 inverted faces
0x0008 if this exists I can't tell what it is.
to get the equalivilent to 'nocull' both 'culling' and 'inverted faces' need to be enabled, see: why most matls have '0x06'.  */

// rpak matl headers
// size of 208
struct MaterialHeaderV12
{
	uint64_t reserved_vtbl; // Gets set to CMaterialGlue vtbl ptr
	uint8_t padding[8]; // unused

	uint64_t guid = 0; // guid of this material asset

	RPakPtr szname; // pointer to partial asset path
	RPakPtr szsurfaceprop; // pointer to surfaceprop (as defined in surfaceproperties.txt)
	RPakPtr szsurfaceprop2; // pointer to surfaceprop2 

	// IDX 1: DepthShadow
	// IDX 2: DepthPrepass
	// IDX 3: DepthVSM
	// IDX 4: ColPass

	uint64_t guidrefs[4]{}; // Required to have proper textures.

	// these blocks relate to different render filters and flags. still not well understood.
	UnknownMaterialSectionV12 unksections[2];

	uint64_t shadersethash; // guid of the shaderset asset that this material uses

	RPakPtr textures; // TextureGUID Map 1

	// both should be reserved - used to store the handles for any textures that have streaming mip levels and the number of textures that have streamed mips
	RPakPtr streamedtextures;
	int16_t streamedtexturescount; // Number of textures with streamed mip levels.

	uint32_t flags = 0x0; // see ImageFlags in the apex struct.
	int16_t unknown2; // name carried over from apex struct.

	uint64_t reserved; // nothing is ever placed here (as I've seen so far), probably reserved for stuff on load.

	// seems to be 0xFBA63181 for loadscreens
	uint32_t unknown3; // name carried over from apex struct.

	uint32_t unknown4; // name carried over from apex struct.

	uint32_t flags2;
	uint32_t something2; // seems mostly unchanged between all materials, including apex, however there are some edge cases where this is 0x0.

	int16_t width;
	int16_t height;
	uint32_t unknown1 = 0; // might be padding but could also be something else such as "m_Unknown1"?.
};

// size of 256
struct MaterialHeaderV15
{
	uint64_t reserved_vtbl; // Gets set to CMaterialGlue vtbl ptr
	uint8_t padding[8]; // unused

	uint64_t guid; // guid of this material asset

	RPakPtr szname; // pointer to partial asset path
	RPakPtr szsurfaceprop; // pointer to surfaceprop (as defined in surfaceproperties.txt)
	RPakPtr szsurfaceprop2; // pointer to surfaceprop2 

	// IDX 1: DepthShadow
	// IDX 2: DepthPrepass
	// IDX 3: DepthVSM
	// IDX 4: DepthShadowTight
	// IDX 5: ColPass

	uint64_t guidrefs[5]{}; // Required to have proper textures.
	uint64_t shadersethash; // guid of the shaderset asset that this material uses

	RPakPtr textures; // TextureGUID Map 1

	// both should be reserved - used to store the handles for any textures that have streaming mip levels and the number of textures that have streamed mips
	RPakPtr streamedtextures;
	int16_t streamedtexturescount; // Number of textures with streamed mip levels.

	int16_t width;
	int16_t height;

	int16_t unknown1 = 0;

	uint32_t flags;
	uint32_t unknown2;

	uint32_t unknown3; // REQUIRED but why?

	uint32_t unknown4;

	// of these 2 seem to be required
	uint32_t flags2 = 0;
	uint32_t something2 = 0;

	UnknownMaterialSectionV15 unksections[2];

	uint8_t bytef0;
	uint8_t bytef1;
	uint8_t bytef2;
	uint8_t bytef3; // used for unksections loading in UpdateMaterialAsset

	uint32_t unk;

	uint64_t txanhash;
};

// header size unchanged all the way until v20, does not mean vars have not changed within
// size of 240
struct MaterialHeaderV16
{
	uint64_t reserved_vtbl; // Gets set to CMaterialGlue vtbl ptr
	uint8_t padding[8]; // unused

	uint64_t guid; // guid of this material asset

	RPakPtr szname; // pointer to partial asset path
	RPakPtr szsurfaceprop; // pointer to surfaceprop (as defined in surfaceproperties.txt)
	RPakPtr szsurfaceprop2; // pointer to surfaceprop2 

	// IDX 1: DepthShadow
	// IDX 2: DepthPrepass
	// IDX 3: DepthVSM
	// IDX 4: DepthShadowTight
	// IDX 5: ColPass

	uint64_t guidrefs[5]{}; // Required to have proper textures.
	uint64_t shadersethash; // guid of the shaderset asset that this material uses

	RPakPtr textures; // TextureGUID Map 1

	// both should be reserved - used to store the handles for any textures that have streaming mip levels and the number of textures that have streamed mips
	RPakPtr streamedtextures;
	int16_t streamedtexturescount; // Number of textures with streamed mip levels.

	int16_t width;
	int16_t height;

	int16_t unknown1 = 0;

	uint32_t flags;
	uint32_t unknown2;

	uint32_t unknown3; // REQUIRED but why?

	uint32_t unknown4;

	// of these 2 seem to be required
	uint32_t flags2 = 0;
	uint32_t something2 = 0;

	UnknownMaterialSectionV15 unksections;

	uint32 unk_v16[2];

	uint8_t bytef0;
	uint8_t bytef1;
	uint8_t bytef2; // used '4' and '8' observed
	uint8_t bytef3; // used for unksections loading in UpdateMaterialAsset

	uint32_t unk;

	uint64_t txanhash;

	uint32_t unk1_v16[6];
};

struct MaterialHeaderV20
{
	uint64_t unk_v20; // not reserved

	uint8_t padding[8]; // unused

	uint64_t guid; // guid of this material asset

	RPakPtr szname; // pointer to partial asset path
	RPakPtr szsurfaceprop; // pointer to surfaceprop (as defined in surfaceproperties.txt)
	RPakPtr szsurfaceprop2; // pointer to surfaceprop2 

	// IDX 1: DepthShadow
	// IDX 2: DepthPrepass
	// IDX 3: DepthVSM
	// IDX 4: DepthShadowTight
	// IDX 5: ColPass

	uint64_t guidrefs[5]{}; // Required to have proper textures.
	uint64_t shadersethash; // guid of the shaderset asset that this material uses

	RPakPtr textures; // TextureGUID Map 1

	// both should be reserved - used to store the handles for any textures that have streaming mip levels and the number of textures that have streamed mips
	RPakPtr streamedtextures;
	int16_t streamedtexturescount; // Number of textures with streamed mip levels.

	int16_t width;
	int16_t height;

	int16_t unknown1 = 0;

	uint32_t flags;
	uint32_t unknown2;

	uint32_t unknown3; // REQUIRED but why?

	uint32_t unknown4;

	// of these 2 seem to be required
	uint32_t flags2 = 0;
	uint32_t something2 = 0;

	UnknownMaterialSectionV15 unksections;

	float unkfloat_v20[2];

	uint8_t bytef0;
	uint8_t bytef1;
	uint8_t bytef2; // used '4' and '8' observed
	uint8_t bytef3; // used for unksections loading in UpdateMaterialAsset

	uint32_t unk; // seen this as '1'

	uint64_t txanhash;

	uint32_t unk1_v16[5];

	float unkfloat1_v20;
};

/* 
flags
0x050300 for loadscreens, 0x1D0300 for normal materials.
0x1D has been observed, seems to invert lighting? used on some exceptionally weird materials.
*/

// matl cpu data
// header struct for the material asset cpu data, with default values.
struct MaterialCPUHeader
{
	RPakPtr data; // data pointer.
	uint32_t size; // size of data buffer.
	uint32_t type; // 0, 2, 3, and 4 observed. might actually be flags but real.
};

struct uvTransformMatrix
{
	// this section is actually broken up into three parts.

	// c_uvRotScaleX
	float uvScaleX;
	float uvRotationX; // rotation, but w e i r d.

	// c_uvRotScaleY
	float uvRotationY; //counter clockwise, 0-1, exceeding one causes Weird Stuff to happen.
	float uvScaleY;

	// c_uvTranslate
	float uvTranslateX;
	float uvTranslateY;
};

// some of these vary so this is just very generic
struct MatlCPUData_V12
{
	// the assignment of these depends on the shader set, they work similarly to texturetransforms in normal source.
	uvTransformMatrix c_uv1; // this is frequently used for detail textures.
	uvTransformMatrix c_uv2;
	uvTransformMatrix c_uv3;

	Vector2 c_uvDistortionIntensity; // distortion on the { x, y } axis.
	Vector2 c_uvDistortion2Intensity; // see above, but for a second distortion texture.

	float c_fogColorFactor;

	float c_layerBlendRamp; // blend intensity (assumed), likely the hardness/softness of the two textures meshing.

	Vector3 c_albedoTint; // color of the albedo texture.
	float c_opacity; // untested.

	float c_useAlphaModulateSpecular;
	float c_alphaEdgeFadeExponent;
	float c_alphaEdgeFadeInner;
	float c_alphaEdgeFadeOuter;

	float c_useAlphaModulateEmissive; // almost always set to 1.
	float c_emissiveEdgeFadeExponent;
	float c_emissiveEdgeFadeInner;
	float c_emissiveEdgeFadeOuter;

	float c_alphaDistanceFadeScale;
	float c_alphaDistanceFadeBias;
	float c_alphaTestReference;

	float c_aspectRatioMulV; // this is equal to width divided by height see: 16/9~, not clear what it actually does.

	Vector3 c_emissiveTint; // color of the emission, this is normally set to { 0.0, 0.0, 0.0 } if you don't have an emission mask.

	float c_shadowBias;

	float c_tsaaDepthAlphaThreshold;
	float c_tsaaMotionAlphaThreshold;
	float c_tsaaMotionAlphaRamp;
	uint32_t c_tsaaResponsiveFlag; // this is 0 or 1 I think.

	float c_dofOpacityLuminanceScale;

	//float c_glitchStrength; // only used  sometimes. on 'Glitch' shadersets, if used 'pad_CBufUberStatic' is only two sections.

	uint32_t pad_CBufUberStatic[3]; // this is reserved space for special values, three sections by default.

	float c_perfGloss;

	Vector3 c_perfSpecColor; // specular color, consistent across most materials.
};

struct MatlCPUData_V15
{
	uvTransformMatrix c_uv1; // detail
	uvTransformMatrix c_uv2; // 1st texture (unconfirmed)
	uvTransformMatrix c_uv3; // 2nd texture (unconfirmed)
	uvTransformMatrix c_uv4;
	uvTransformMatrix c_uv5;

	Vector2 c_uvDistortionIntensity;
	Vector2 c_uvDistortion2Intensity;

	float c_L0_scatterDistanceScale;

	float c_layerBlendRamp;

	float c_opacity;

	float c_useAlphaModulateSpecular;
	float c_alphaEdgeFadeExponent;
	float c_alphaEdgeFadeInner;
	float c_alphaEdgeFadeOuter;

	float c_useAlphaModulateEmissive;
	float c_emissiveEdgeFadeExponent;
	float c_emissiveEdgeFadeInner;
	float c_emissiveEdgeFadeOuter;

	float c_alphaDistanceFadeScale;
	float c_alphaDistanceFadeBias;
	float c_alphaTestReference;

	float c_aspectRatioMulV;

	float c_shadowBias;
	float c_shadowBiasStatic;

	float c_dofOpacityLuminanceScale;

	float c_tsaaDepthAlphaThreshold;
	float c_tsaaMotionAlphaThreshold;
	float c_tsaaMotionAlphaRamp;
	uint32_t c_tsaaResponsiveFlag; // this is 0 or 1 I think.

	Vector3 c_outlineColorSDF;
	float c_outlineWidthSDF;

	Vector3 c_shadowColorSDF;
	float c_shadowWidthSDF;

	Vector3 c_insideColorSDF;

	float c_outsideAlphaScalarSDF;

	float c_glitchStrength;

	float c_vertexDisplacementScale;

	float c_innerFalloffWidthSDF;
	float c_innerEdgeOffsetSDF;

	Vector2 c_dropShadowOffsetSDF;

	float c_normalMapEdgeWidthSDF;

	float c_shadowFalloffSDF;

	Vector3 c_L0_scatterAmount;
	float c_L0_scatterRatio;

	float c_L0_transmittanceIntensityScale;

	Vector3 c_vertexDisplacementDirection;

	float c_L0_transmittanceAmount;
	float c_L0_transmittanceDistortionAmount;

	float c_zUpBlendingMinAngleCos;
	float c_zUpBlendingMaxAngleCos;
	float c_zUpBlendingVertexAlpha;

	Vector3 c_L0_albedoTint;

	float c_depthBlendScalar;

	Vector3 c_L0_emissiveTint;

	float c_subsurfaceMaterialID;

	Vector3 c_L0_perfSpecColor;

	float c_L0_perfGloss;

	Vector3 c_L1_albedoTint;

	float c_L1_perfGloss;

	Vector3 c_L1_emissiveTint;
	Vector3 c_L1_perfSpecColor;

	float c_splineMinPixelPercent;

	Vector2 c_L0_anisoSpecCosSinTheta;
	Vector2 c_L1_anisoSpecCosSinTheta;

	float c_L0_anisoSpecStretchAmount;
	float c_L1_anisoSpecStretchAmount;

	float c_L0_emissiveHeightFalloff;
	float c_L1_emissiveHeightFalloff;

	float c_L1_transmittanceIntensityScale;
	float c_L1_transmittanceAmount;
	float c_L1_transmittanceDistortionAmount;

	float c_L1_scatterDistanceScale;
	Vector3 c_L1_scatterAmount;
	float c_L1_scatterRatio;
};

// size of 128
struct MatlCPUDataSpriteCard_V16
{
	float c_sizeFactorMin;
	float c_sizeFactorMax;
	float c_sizeFactorFadeStart;
	float c_sizeFactorFadeEnd;

	float c_farFadeStart;
	float c_farFadeFactor;

	float c_fogScaleFactor;
	float c_fogColorFactor;

	float c_depthFeatherScale;

	float c_overbrightFactor;

	float c_additiveBlendWeight;

	float c_staticBufPadding0;

	float c_dof_opacity_scale;
	float c_dof_opacity_scale_threshold;

	float c_alphaDistanceFadeScale;
	float c_alphaDistanceFadeBias;
	float c_alphaTestRef;

	float c_tsaaMotionAlphaThreshold;
	float c_tsaaMotionAlphaRamp;
	uint32_t c_tsaaResponsiveFlag; // this is 0 or 1 I think.

	uvTransformMatrix c_uv1;
	uvTransformMatrix c_uv2;
};

// size of 192
struct MatlCPUDataRefract_V16
{
	uvTransformMatrix c_uv1;
	uvTransformMatrix c_uv2;

	Vector3 c_refractTint;
	float c_refractScale;

	Vector2 c_refractDepthTimesTextureAspectFixup;
	Vector2 c_halfPixelOffset;
	float c_fogColorFactor;

	Vector3 c_bumpDecodeScale;
	Vector3 c_bumpDecodeBias;

	float c_envmapSaturation;
	Vector3 c_envmapTint;
	float c_envmapContrast;

	Vector2 c_magnifyCenter;
	float c_inverseMagnifyScale;

	float c_unused_0;
	float c_unused_1;

	Vector3 c_bump2DecodeScale;
	Vector3 c_bump2DecodeBias;

	float c_unused_2;

	float c_tsaaMotionAlphaThreshold;
	float c_tsaaMotionAlphaRamp;
	uint32_t c_tsaaResponsiveFlag; // this is 0 or 1 I think.

	float c_unused_3;
};

// size of 576
struct MatlCPUData_V16
{
	uvTransformMatrix c_uv1; // detail
	uvTransformMatrix c_uv2; // 1st texture (unconfirmed)
	uvTransformMatrix c_uv3; // 2nd texture (unconfirmed)
	uvTransformMatrix c_uv4;
	uvTransformMatrix c_uv5;

	Vector2 c_uvDistortionIntensity;
	Vector2 c_uvDistortion2Intensity;

	float c_L0_scatterDistanceScale;

	float c_layerBlendRamp;

	float c_opacity;

	float c_useAlphaModulateSpecular;
	float c_alphaEdgeFadeExponent;
	float c_alphaEdgeFadeInner;
	float c_alphaEdgeFadeOuter;

	float c_useAlphaModulateEmissive;
	float c_emissiveEdgeFadeExponent;
	float c_emissiveEdgeFadeInner;
	float c_emissiveEdgeFadeOuter;

	float c_alphaDistanceFadeScale;
	float c_alphaDistanceFadeBias;
	float c_alphaTestReference;

	float c_aspectRatioMulV;

	float c_shadowBias;
	float c_shadowBiasStatic;

	float c_dofOpacityLuminanceScale;

	float c_tsaaDepthAlphaThreshold;
	float c_tsaaMotionAlphaThreshold;
	float c_tsaaMotionAlphaRamp;
	uint32_t c_tsaaResponsiveFlag; // this is 0 or 1 I think.

	Vector3 c_outlineColorSDF;
	float c_outlineWidthSDF;

	Vector3 c_shadowColorSDF;
	float c_shadowWidthSDF;

	Vector3 c_insideColorSDF;

	float c_outsideAlphaScalarSDF;

	float c_glitchStrength;

	float c_vertexDisplacementScale;

	float c_innerFalloffWidthSDF;
	float c_innerEdgeOffsetSDF;

	Vector2 c_dropShadowOffsetSDF;

	float c_normalMapEdgeWidthSDF;

	float c_shadowFalloffSDF;

	Vector3 c_L0_scatterAmount;
	float c_L0_scatterRatio;

	float c_L0_transmittanceIntensityScale;

	Vector3 c_vertexDisplacementDirection;

	float c_L0_transmittanceAmount;
	float c_L0_transmittanceDistortionAmount;

	float c_zUpBlendingMinAngleCos;
	float c_zUpBlendingMaxAngleCos;
	float c_zUpBlendingVertexAlpha;

	Vector3 c_L0_albedoTint;

	float c_depthBlendScalar;

	Vector3 c_L0_emissiveTint;

	float c_subsurfaceMaterialID;

	Vector3 c_L0_perfSpecColor;

	float c_L0_perfGloss;

	Vector3 c_L1_albedoTint;

	float c_L1_perfGloss;

	Vector3 c_L1_emissiveTint;
	Vector3 c_L1_perfSpecColor;

	float c_splineMinPixelPercent;

	Vector2 c_L0_anisoSpecCosSinTheta;
	Vector2 c_L1_anisoSpecCosSinTheta;

	float c_L0_anisoSpecStretchAmount;
	float c_L1_anisoSpecStretchAmount;

	float c_L0_emissiveHeightFalloff;
	float c_L1_emissiveHeightFalloff;

	float c_L1_transmittanceIntensityScale;
	float c_L1_transmittanceAmount;
	float c_L1_transmittanceDistortionAmount;

	float c_L1_scatterDistanceScale;
	Vector3 c_L1_scatterAmount;
	float c_L1_scatterRatio;

	Vector3 c_characterBoostTint;
	float c_characterBoostFogScale;
	Vector3 c_edgeDetectOutlineColor;

	float c_edgeDetectOutlineAlpha;
	float c_edgeDetectOutlineWidth;
	float c_edgeDetectOutlineFalloffExp;

	float c_characterBoostScale;
	float c_characterBoostBias;

	float c_alphaErosionHardness;

	Vector3 c_uberStaticPadding0;
};

// cba to do v17 and v20 currently
