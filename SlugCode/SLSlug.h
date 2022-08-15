//
// Slug Library, version 6.1
//
// This is the primary header file for the Slug GPU font rendering library.
// The contents of this file are confidential and may be distributed only to authorized users.
//
// Slug was developed by Eric Lengyel at Terathon Software LLC.
// Copyright 2016-2022.
//
// Slug is protected by United States Patent #10373352, issued on August 6, 2019:
// "Method for rendering resolution-independent shapes directly from outline control points".
//


#ifndef SLSlug_h
#define SLSlug_h


#include "TSPlatform.h"


#define TERATHON_SLUG 1


#define SLUG_NO_LEGACY		// Comment out this line to include compatibility code in the library.


namespace Terathon
{
	#ifndef TERATHON_HALF

		typedef uint16 Half;

	#endif

	#ifndef TERATHON_INTEGER2D

		class Integer2D
		{
			public:

				int32		x;
				int32		y;
		};

	#endif

	#ifndef TERATHON_INTEGER4D

		class Integer4D
		{
			public:

				int32		x;
				int32		y;
				int32		z;
				int32		w;
		};

	#endif

	#ifndef TERATHON_VECTOR2D

		class Vector2D
		{
			public:

				float		x;
				float		y;
		};

	#endif

	#ifndef TERATHON_POINT2D

		class Point2D
		{
			public:

				float		x;
				float		y;
		};

	#endif

	#ifndef TERATHON_MATRIX2D

		class Matrix2D
		{
			public:

				float		m00;
				float		m10;
				float		m01;
				float		m11;
		};

	#endif

	#ifndef TERATHON_POINT3D

		class Point3D
		{
			public:

				float		x;
				float		y;
				float		z;
		};

	#endif

	#ifndef TERATHON_BIVECTOR3D

		class Bivector3D
		{
			public:

				float		x;
				float		y;
				float		z;
		};

	#endif

	#ifndef TERATHON_VECTOR4D

		class Vector4D
		{
			public:

				float		x;
				float		y;
				float		z;
				float		w;
		};

	#endif

	#ifndef TERATHON_BOX2D

		class Box2D
		{
			public:

				Point2D		min;
				Point2D		max;
		};

	#endif

	#ifndef TERATHON_BEZIER

		class QuadraticBezier2D
		{
			public:

				Point2D		p[3];
		};

	#endif

	#ifndef TERATHON_COLOR1U

		typedef uint8 Color1U;

	#endif

	#ifndef TERATHON_COLOR4U

		class Color4U
		{
			public:

				uint8		red;
				uint8		green;
				uint8		blue;
				uint8		alpha;
		};

	#endif


	namespace Slug
	{
		typedef uint32 FontKeyType;			// Font key types are identified by four-character codes.
		typedef uint32 AlignmentType;		// Alignment types are identified by four-character codes.
		typedef uint32 GeometryType;		// Geometry types are identified by four-character codes.
		typedef uint32 EffectType;			// Effect types are identified by four-character codes.
		typedef uint32 StrokeCapType;		// Stroke cap types are identified by four-character codes.
		typedef uint32 StrokeJoinType;		// Stroke join types are identified by four-character codes.
		typedef uint32 GradientType;		// Gradient types are identified by four-character codes.
		typedef uint32 TextureType;			// Texture types are identified by four-character codes.


		constexpr float kDefaultStrokeWidth = 1.0F;
		constexpr float kDefaultMiterLimit = 4.0F;

		constexpr float kMaxGlyphControlCoord = 65536.0F;
		constexpr float kGlyphBandEpsilon = 1.0F / 1024.0F;


		enum
		{
			kUnicodePlaneCount					= 17,
			kUnicodePointCount					= kUnicodePlaneCount * 0x010000,
			kUnicodeCharDefCount				= 0x030000
		};


		enum : uint32
		{
			kCharacterLeftToRight				= 1 << 0,
			kCharacterRightToLeft				= 1 << 1,
			kCharacterJoinNext					= 1 << 2,
			kCharacterJoinPrevious				= 1 << 3,
			kCharacterJoinIgnore				= 1 << 4,
			kCharacterBidiMirror				= 1 << 5,
			kCharacterInvisible					= 1 << 6
		};


		enum
		{
			kMaxStringCharacterCount			= 16383,		// The maximum number of Unicode characters that can be processed in a single string. The actual number of bytes occupied by each character in the UTF-8 encoding does not matter. Embedded format directives do not count against this limit.
			kMaxStringLayoutDataCount			= 256,			// The maximum number of sets of embedded format directives allowed in a single string, where one set is delimited by {# and }. The number of directives in each set does not matter. Adjacent sets with no intervening characters count as one.
			kMaxStringRunDataCount				= 256,			// The maximum number of distinct directional runs allowed in a single string. There is always at least one run associated with the beginning of the string, and a new run is started whenever the writing direction changes within the string.
			kMaxStringFontCount					= 256			// The maximum number of fonts used in a single string.
		};


		enum
		{
			kMaxFillBandCount					= 32,			// The maximum number of bands allowed for a single fill.
			kMaxFillCurveCount					= 8192,			// The maximum number of quadratic curves allowed in a single fill.
			kMaxStrokeCurveCount				= 8192			// The maximum number of quadratic curves allowed in a single stroke.
		};


		enum
		{
			kMaxCurveTextureWidth				= 4096,
			kMaxCurveTextureHeight				= 2048,
			kMaxBandTextureWidth				= 4096,
			kMaxBandTextureHeight				= 8192
		};


		// The following values give the maximum number of unique vertex shaders and fragment shaders that can ever be used
		// by Slug. The indices returned by the GetShaderIndices() function are always less than these values.

		enum
		{
			kMaxVertexShaderCount				= 1,
			kMaxFragmentShaderCount				= 256
		};


		// The following values give the maximum number of strings that can be returned by the GetVertexShaderSourceCode()
		// and GetFragmentShaderSourceCode() functions.

		enum
		{
			kMaxVertexStringCount				= 3,
			kMaxFragmentStringCount				= 10
		};


		// The following values give the maximum number of quadratic Bézier curves that can be generated by the GenerateRectangleCurves()
		// and GenerateCircleCurves() functions.

		enum
		{
			kMaxRectangleCurveCount				= 16,
			kMaxCircleCurveCount				= 12
		};


		enum : uint32
		{
			kVertexShaderProlog					= 1 << 0,		// A prolog component containing type definitions for compatibility across different shading languages.
			kVertexShaderMain					= 1 << 1,		// The component containing a main() function and declarations for vertex attributes and uniform parameters.

			kVertexShaderDefault				= kVertexShaderProlog | kVertexShaderMain
		};


		enum : uint32
		{
			kFragmentShaderProlog				= 1 << 0,		// A prolog component containing type definitions for compatibility across different shading languages.
			kFragmentShaderMain					= 1 << 1,		// The component containing a main() function and declarations for interpolants and texture maps.

			kFragmentShaderDefault				= kFragmentShaderProlog | kFragmentShaderMain
		};


		// The following values can be combined (through logical OR) in the graphicFlags field of the GraphicData structure.

		enum : uint32
		{
			kGlyphMark							= 1 << 0,		// The glyph is a combining mark.
			kIconLinear							= 1 << 8,		// All curves composing the icon are straight lines.

			kIconGradientMask					= 0xF000
		};


		// The following values can be combined (through logical OR) in the pictureFlags field of the PictureData structure.

		enum : uint32
		{
			kPictureLinearFills					= 1 << 0,		// The picture contains at least one fill that is defined entirely by linear curves. The kRenderLinearCurves flag may be specified for the GetShaderIndices() function to enable the shader optimization, but it is not required.
			kPictureStrokes						= 1 << 1,		// The picture contains at least one stroke. The kRenderStrokes flag must be specified for the GetShaderIndices() function in order to render the picture. The kRenderStrokes flag is not optional, and it is not the case that strokes simply will not be rendered without it.
			kPictureGradients					= 1 << 2		// The picture contains at least one gradient. The kRenderGradients flag should be specified for the GetShaderIndices() function in order to render the gradients. The kRenderGradients flag is not strictly required, and gradient fills will simply be rendered as solid colors without it.
		};


		// The following values can be combined (through logical OR) in the fontFlags field of the FontHeader structure.

		enum : uint32
		{
			kFontMulticolor						= 1 << 0,		// The font contains data for multicolor glyphs.
			kFontOutline						= 1 << 1,		// The font contains data for outlined glyph effects.
			kFontUnderline						= 1 << 2,		// The font contains synthesized underline glyphs.
			kFontStrikethrough					= 1 << 3		// The font contains synthesized strikethrough glyphs.
		};


		// The following values can be combined (through logical OR) in the albumFlags field of the AlbumHeader structure.

		enum : uint32
		{
			kAlbumCurveTextureFloat32			= 1 << 0		// The album's curve texture has 32-bit floating-point components. If this flag is not set, then the curve texture has 16-bit floating-point components.
		};


		// The following values are the keys that can be passed to the GetFontKeyData() function.

		enum : FontKeyType
		{
			kFontKeyName						= 'NAME',		// The font name string.
			kFontKeySubname						= 'SNAM',		// The font subname string.
			kFontKeyMetrics						= 'MTRC',		// Data for the default metrics.
			kFontKeyTypoMetrics					= 'TYPO',		// Data for the typographic metrics.
			kFontKeyHeight						= 'HITE',		// Data for the cap height and ex height.
			kFontKeyBoundingBox					= 'BBOX',		// Data for the bounding box limits.
			kFontKeySubscript					= 'SUBS',		// Data for the subscript transform.
			kFontKeySuperscript					= 'SUPS',		// Data for the superscript transform.
			kFontKeyUnderline					= 'UNDR',		// Data for the underline position and size.
			kFontKeyStrikethrough				= 'STRK',		// Data for the strikethrough position and size.
			kFontKeyPolygon						= 'POLY',		// Data for the glyph bounding polygons.
			kFontKeyOutline						= 'OTLN'		// Data for the outline effect.
		};


		// The following values can be combined (through logical OR) with any hard break character passed to the
		// BreakSlug() or BreakMultiLineText() function to specify special handling.

		enum : uint32
		{
			kBreakSameParagraph					= 1 << 24,		// The break does not start a new paragraph.
			kBreakCombineNext					= 1 << 25		// If the next character is a hard break having a different Unicode value, then the two breaks are combined into one.
		};


		// The following values can be combined (through logical OR) in the lineFlags field of the LineData structures
		// produced by the BreakSlug() and BreakMultiLineText() functions.

		enum : uint32
		{
			kLineParagraphLast					= 1 << 0		// The line is the last line in a paragraph, and the next line (if any) starts a new paragraph.
		};


		// The following values can be combined (through logical OR) in the sequenceMask field of the LayoutData structure.
		// The sequence type mask is ignored if the kLayoutSequenceDisable bit is set in the layoutFlags field.

		enum : uint32
		{
			kSequenceGlyphComposition			= 1 << 0,		// Glyph compositions expected by a font to be applied in all circumstances. This is used for emoji with skin tone modifiers, for example.
			kSequenceStandardLigatures			= 1 << 1,		// Standard ligatures provided by a font as substitute glyphs for letter groupings such as "fi" or "ffl". (Some fonts may specify these as discretionary.)
			kSequenceRequiredLigatures			= 1 << 2,		// Required ligatures considered by a font to be required in some writing systems for various letter groupings.
			kSequenceDiscretionaryLigatures		= 1 << 3,		// Discretionary ligatures provided by a font as additional substitute glyphs that are considered optional.
			kSequenceHistoricalLigatures		= 1 << 4,		// Historical ligatures provided by a font as optional antiquated forms.
			kSequenceAlternativeFractions		= 1 << 8,		// Alternative fractions provided by a font for specific numerators and denominators separated by a slash.

			kSequenceDefaultMask				= kSequenceGlyphComposition | kSequenceStandardLigatures | kSequenceRequiredLigatures
		};


		// The following values can be combined (through logical OR) in the alternateMask field of the LayoutData structure.
		// The alternate type mask is ignored if the kLayoutAlternateDisable bit is set in the layoutFlags field.

		enum : uint32
		{
			kAlternateStylistic					= 1 << 0,		// Replace glyphs with alternates from a stylistic set. The styleIndex field of the LayoutData structure determines which set is used.
			kAlternateHistorical				= 1 << 1,		// Replace glyphs with historical alternates.
			kAlternateLowerSmallCaps			= 1 << 2,		// Replace lowercase characters with small caps variants.
			kAlternateUpperSmallCaps			= 1 << 3,		// Replace uppercase characters with small caps variants.
			kAlternateTitlingCaps				= 1 << 4,		// Replace glyphs with titling caps variants.
			kAlternateUnicase					= 1 << 5,		// Replace both cases with forms having equal heights.
			kAlternateCaseForms					= 1 << 6,		// Replace case-sensitive punctuation with uppercase forms.
			kAlternateSlashedZero				= 1 << 7,		// Replace the number zero with a slashed variant.
			kAlternateHyphenMinus				= 1 << 8,		// Replace the hyphen (U+002D) with a minus sign (U+2212).
			kAlternateFractions					= 1 << 9,		// Replace figures separated by a slash with numerators, the fraction slash, and denominators.
			kAlternateLiningFigures				= 1 << 16,		// Replace oldstyle figures with lining figures.
			kAlternateOldstyleFigures			= 1 << 17,		// Replace lining figures with oldstyle figures.
			kAlternateTabularFigures			= 1 << 18,		// Replace proportional figures with tabular figures.
			kAlternateProportionalFigures		= 1 << 19,		// Replace tabular figures with proportional figures.
			kAlternateSubscript					= 1 << 20,		// Replace glyphs with subscript variants.
			kAlternateSuperscript				= 1 << 21,		// Replace glyphs with superscript variants.
			kAlternateInferiors					= 1 << 22,		// Replace glyphs with subscripts intended for scientific formulas.
			kAlternateOrdinals					= 1 << 23		// Replace glyphs with superscripts intended for ordinal numbers.
		};


		// The following values can be combined (through logical OR) in the layoutFlags field of the LayoutData structure.

		enum : uint32
		{
			kLayoutFormatDirectives				= 1 << 0,		// Format directives embedded in the text are applied.
			kLayoutClippingPlanes				= 1 << 1,		// Each glyph is clipped against left and right boundaries.
			kLayoutKernDisable					= 1 << 4,		// Kerning is not applied to the text. When this flag is not set, kerning is applied according to the tables specified in the original font.
			kLayoutMarkDisable					= 1 << 5,		// Combining marks are not repositioned in the text. When this flag is not set, each combining mark is moved to the appropriate attachment point belonging to the preceding glyph.
			kLayoutSequenceDisable				= 1 << 6,		// Sequences are not matched in the text. When this flag is not set, tables specified in the original font can cause certain sequences of glyphs to be replaced by one or more other glyphs.
			kLayoutAlternateDisable				= 1 << 7,		// Alternate glyph substitution is not applied in the text. When this flag is not set, various substitution features can cause glyphs to be replaced by alternate forms.
			kLayoutLayerDisable					= 1 << 8,		// Only monochrome glyphs are rendered, and color layers are ignored. This flag applies only if the kRenderMulticolor flag is not specified in the renderFlags field of the LayoutData structure.
			kLayoutRightToLeft					= 1 << 12,		// The forward writing direction is right-to-left.
			kLayoutBidirectional				= 1 << 13,		// Bidirectional text layout is enabled.
			kLayoutGridPositioning				= 1 << 16,		// Grid positioning mode is enabled.
			kLayoutParagraphAttributes			= 1 << 17,		// Paragraph spacing, margins, and first-line indent are enabled.
			kLayoutTabSpacing					= 1 << 18		// Tab spacing is enabled.
		};


		// The following values can be combined (through logical OR) in the renderFlags parameter of the GetShaderIndices() function.

		enum : uint32
		{
			kRenderMulticolor					= 1 << 0,		// An icon is rendered using a single-pass multicolor shader. The output color is always premultiplied by the coverage. This flag cannot be specified for glyphs.
			kRenderSymmetricBands				= 1 << 1,		// The symmetric band split optimization is applied in the fragment shader.
			kRenderOpticalWeight				= 1 << 2,		// When rendering, coverage values are remapped to increase the optical weight of the glyphs. This can improve the appearance of small text, but usually looks good only for dark text on a light background.
			kRenderSupersampling				= 1 << 3,		// The fragment shader performs adaptive supersampling for high-quality rendering at small font sizes.
			kRenderLinearCurves					= 1 << 4,		// The fragment shader checks whether an icon or component of a picture is composed of straight lines and chooses a faster path for them when possible.
			kRenderStrokes						= 1 << 5,		// The fragment shader is capable of rendering strokes. This flag is required when strokes are rendered individually or as part of a picture.
			kRenderGradients					= 1 << 6,		// The fragment shader is capable of rendering gradients. If this flag is not set, then all gradient fills are rendered with solid colors instead.
			kRenderPremultiplyCoverage			= 1 << 8,		// The color output by the fragment shader is premultiplied by the coverage. This flag is redundant if the kRenderMulticolor flag is specified.
			kRenderPremultiplyInverseCoverage	= 1 << 9		// The color output by the fragment shader is premultiplied by the inverse of the coverage. This flag is ignored if either the kRenderMulticolor or kRenderPremultiplyCoverage flag is specified.
		};


		// The following values can be combined (through logical OR) in the formatMask field of the LayoutData structure.
		// The format mask is used only when the kLayoutFormatDirectives bit is set in the layoutFlags field.

		enum : uint32
		{
			kFormatFont							= 1 << 0,		// Font type directive font().
			kFormatSize							= 1 << 1,		// Font size directive size().
			kFormatStretch						= 1 << 2,		// Font stretch directive stretch().
			kFormatTracking						= 1 << 3,		// Text tracking directive track().
			kFormatSkew							= 1 << 4,		// Text skew directive skew().
			kFormatScale						= 1 << 5,		// Text scale directive scale().
			kFormatOffset						= 1 << 6,		// Text offset directive offset().
			kFormatColor						= 1 << 7,		// Color directives color(), color2(), effect_color(), and effect_color2().
			kFormatGradient						= 1 << 8,		// Gradient directives grad(), gcoord(), effect_grad(), and effect_gcoord().
			kFormatAlignment					= 1 << 9,		// Alignment directives left(), right(), and center().
			kFormatLeading						= 1 << 10,		// Leading directive lead().
			kFormatParagraph					= 1 << 11,		// Paragraph directives pspace(), margin(), and indent().
			kFormatTab							= 1 << 12,		// Tab size directive tab().
			kFormatKern							= 1 << 13,		// Kerning directive kern().
			kFormatMark							= 1 << 14,		// Mark placement directive mark().
			kFormatSequence						= 1 << 15,		// Sequence directive seq() and all specific sequence replacement directives.
			kFormatAlternate					= 1 << 16,		// Alternate directive alt() and all specific alternate substitution directives.
			kFormatLayer						= 1 << 17,		// Color layer directive lay().
			kFormatDecoration					= 1 << 18,		// Underline directive under() and strikethrough directive strike().
			kFormatScript						= 1 << 19,		// Transform-based subscript and superscript directive script().
			kFormatGrid							= 1 << 20		// Grid positioning directive grid().
		};


		// The following values are used as flags in the high 12 bits of the extendedData field of the GlyphData structure.

		enum : uint32
		{
			kExtendedGlyphOffset				= 1 << 20,		// The extended data contains a nonzero glyph offset.
			kExtendedCaretData					= 1 << 21		// The extended data contains caret positioning values for a ligature.
		};


		enum : uint32
		{
			kTerminatorGlyph					= 0x00FFFFFF
		};


		// The following values are array indexes that can be used in the
		// decorationFlag field of the LayoutData structure.

		enum
		{
			kDecorationUnderline				= 0,			// Underline decoration.
			kDecorationStrikethrough			= 1,			// Strikethrough decoration.
			kDecorationCount					= 2
		};


		enum
		{
			kFractionSlash						= 1,
			kFractionNumerator					= 2,
			kFractionDenominator				= 3
		};


		enum
		{
			kCursiveFormInitial					= 1,
			kCursiveFormMedial					= 2,
			kCursiveFormFinal					= 3,
			kCursiveFormIsolated				= 4
		};


		// The following values are the alignment types that can be specified
		// in the textAlignment field of the LayoutData structure.

		enum : AlignmentType
		{
			kAlignmentLeft						= 'LEFT',		// Text is aligned with the rendering position on the left end of each line.
			kAlignmentRight						= 'RGHT',		// Text is aligned with the rendering position on the right end of each line.
			kAlignmentCenter					= 'CENT'		// Text is aligned with the rendering position at the center of each line.
		};


		// The following values are the geometry types that can be specified
		// in the geometryType field of the LayoutData structure.

		enum : GeometryType
		{
			kGeometryQuads						= 'QUAD',		// Each glyph is rendered with a single quad composed of four vertices and two triangles.
			kGeometryPolygons					= 'POLY',		// Each glyph is rendered with a tight bounding polygon having between 4 and 8 vertices. If polygon data is not available in the font, then each glyph is rendered as a quad. 
			kGeometryRectangles					= 'RECT'		// Each glyph is rendered with exactly one triangle with the expectation that the window-aligned bounding rectangle will be filled. This type can be used only when rectangle primitives are available, such as provided by the VK_NV_fill_rectangle extension in Vulkan.
		};


		// The following values are the effect types that can be specified
		// in the effectType field of the LayoutData structure.

		enum : EffectType
		{
			kEffectNone							= 0,			// No glyph effect is applied.
			kEffectShadow						= 'SHAD',		// A drop shadow is rendered beneath each glyph.
			kEffectOutline						= 'OTLN',		// An expanded outline is rendered beneath each glyph. (The font must contain outline glyph effect data for this effect to appear.)
			kEffectCount						= 3
		};


		// The following values can be combined (through logical OR) in the strokeFlags field of the StrokeData structure.

		enum : uint32
		{
			kStrokeClosed						= 1 << 0		// Strokes form a closed path.
		};


		// The following values are the types of caps that can be applied to strokes.

		enum : StrokeCapType
		{
			kStrokeCapFlat						= 'FLAT',		// Strokes do not have caps.
			kStrokeCapTriangle					= 'TRIA',		// Strokes have triangular caps.
			kStrokeCapSquare					= 'SQUA',		// Strokes have square caps.
			kStrokeCapRound						= 'ROND'		// Strokes have round caps.
		};


		// The following values are the types of joining that can be applied when two strokes meet
		// at an angle that causes the miter limit to be exceeded.

		enum : StrokeJoinType
		{
			kStrokeJoinBevel					= 'BEVL',		// Strokes are joined by beveled corners.
			kStrokeJoinRound					= 'ROND'		// Strokes are joined by rounded corners.
		};


		// The following values are the types of gradients that can applied to fills.

		enum : GradientType
		{
			kGradientNone						= 0,			// No gradient is applied.
			kGradientLinear						= 'LINR',		// The gradient is linear with respect to a scaled line.
			kGradientRadial						= 'RADL'		// The gradient is radial with respect to a center and radius.
		};


		// The following values are the texture formats that can be used for curve and band textures.

		enum : TextureType
		{
			kTextureDefault						= 0,			// The texture uses the default format, which is kTextureFloat16 for curve textures and kTextureUint16 for band textures.
			kTextureFloat16						= 'HLF4',		// Each of the four channels of the texture contains a 16-bit float-point value (curve textures only).
			kTextureFloat32						= 'FLT4',		// Each of the four channels of the texture contains a 32-bit float-point value (curve textures only).
			kTextureUint16						= 'UIN4'		// Each of the four channels of the texture contains a 16-bit unsigned integer (band textures only).
		};


		union Texel16
		{
			Half		x[4];
			int32		z[2];
			float		f[2];
			uint16		u[4];
		};


		union Texel32
		{
			float		x[4];
			int64		z[2];
		};


		// The font key data table contains an array of FontKeyData structures.

		struct FontKeyData
		{
			FontKeyType			keyValue;						// The key value identifying the type of data.
			int32				dataOffset;						// The offset from this structure to the data.

			const void *GetData(void) const
			{
				return (reinterpret_cast<const char *>(this) + dataOffset);
			}
		};


		// The FontMetricsData structure is returned by the GetFontKeyData() function for the kFontKeyMetrics
		// and kFontKeyTypoMetrics keys.

		struct FontMetricsData
		{
			float				metricAscent;					// The font designer's suggested ascent for the font, in em units. This is a positive distance above the baseline representing the vertical space occupied by the font.
			float				metricDescent;					// The font designer's suggested descent for the font, in em units. This is a negative distance below the baseline representing the vertical space occupied by the font.
			float				metricLineGap;					// The font designer's suggested line gap for the font, in em units. This is a positive distance added to the difference between the ascent and descent to calculate a suggested leading.
		};


		// The FontHeightData structure is returned by the GetFontKeyData() function for the kFontKeyHeight key.

		struct FontHeightData
		{
			float				fontCapHeight;					// The cap height for the font, in em units. This represents the typical distance from the baseline to the tops of the capital roman letters.
			float				fontExHeight;					// The ex height for the font, in em units. This represents the typical distance from the baseline to the tops of lowercase roman letters, disregarding ascenders.
		};


		// The FontBoundingBoxData structure is returned by the GetFontKeyData() function for the kFontKeyBoundingBox key.

		struct FontBoundingBoxData
		{
			Box2D				baseBoundingBox;				// The font-wide union of all em-space bounding boxes for base glyphs, which excludes combining marks.
			Box2D				markBoundingBox;				// The font-wide union of all em-space bounding boxes for glyphs that are combining marks. If a font does not contain any combining marks, then all coordinates for this box are zero.
		};


		// The FontScriptData structure is returned by the GetFontKeyData() function for the kFontKeySubscript
		// and kFontKeySuperscript keys.

		struct FontScriptData
		{
			Vector2D			scriptScale;					// The scale for script glyphs corresponding to the textScale field of the LayoutData structure.
			Vector2D			scriptOffset;					// The offset for script glyphs corresponding to the textOffset field of the LayoutData structure. 
		};


		// The FontDecorationData structure is returned by the GetFontKeyData() function for the kFontKeyUnderline
		// and kFontKeyStrikethrough keys.

		struct FontDecorationData
		{
			float				decorationSize;					// The thickness of the decoration stroke, in em units.
			float				decorationPosition;				// The em-space y position of the bottom edge of the decoration stroke.
			uint16				dataLocation[2];				// The coordinates in the font's band texture at which data for the decoration begins.
		};


		// The FontPolygonData structure is returned by the GetFontKeyData() function for the kFontKeyPolygon key.

		struct FontPolygonData
		{
			int32				polygonVertexCount;				// The maximum number of vertices that the bounding polygon for any glyph can have.
			float				polygonEdgeFactor;				// The cost multiplier applied to the interior edges of a polygon's triangulation.
		};


		// The FontOutlineData structure is returned by the GetFontKeyData() function for the kFontKeyOutline key.

		struct FontOutlineData
		{
			float				outlineEffectSize;				// The size by which glyphs are expanded for the outline effect, in em units.
			float				outlineMiterLimit;				// The miter limit used for the outline effect where curves meet.
			StrokeJoinType		outlineJoinType;				// The join type used when the miter limit is exceeded in the outline effect.
		};


		// The GraphicData structure contains properties common to both glyphs and icons.

		struct GraphicData
		{
			uint16				graphicFlags;					// Flags indicating various properties of the glyph or icon.
			uint8				bandCount[2];					// The number of vertical and horizontal bands holding curve index data for the graphic. These are zero if the graphic has no curves.
			Vector2D			bandScale;						// The x and y scale by which em-space coordinates are multiplied to calculate band indices.
			Box2D				boundingBox;					// The bounding box of the graphic in the coordinate system of the em square.
			float				advanceWidth;					// The advance width of the graphic in em units.
			uint16				dataLocation[2];				// The coordinates in the band texture at which data for the graphic begins.
		};


		// Each glyph in a font has an associated GlyphData structure that describes all of the glyph's properties.

		struct GlyphData : GraphicData
		{
			uint32				extendedData;					// The high 12 bits contain flags indicating what extended data is present, and the low 20 bits contain the offset into the font's extended data table at which the data begins for the glyph.
			uint32				geometryData;					// The high 8 bits contain the geometry code for the glyph, and the low 24 bits contain the offset into the font's geometry data table at which vertex coordinates begin for the glyph.
			uint32				kernData;						// The high 12 bits contain the number of kern pairs for which the glyph is on the right side of each pair, and the low 20 bits contain the offset into the font's kerning data table at which kern pairs begin for the glyph.
			uint32				markData;						// The high 6 bits contain the number of anchor points to which combining marks can be attached to the glyph. The next 6 bits, nonzero only for mark glyphs, contain the number of anchor points at which a mark can attach to another glyph. The low 20 bits contain the offset into the font's combining mark data table at which anchor points begin for the glyph. The number of elements in the table is equal to the sum of the two counts in the high 12 bits.
			uint32				sequenceData;					// The high 12 bits contain the number of sequences for the glyph, and the low 20 bits contain the offset into the font's sequence data table at which sequences begin for the glyph.
			uint32				alternateData;					// The high 8 bits contain the number of alternates for the glyph, and the low 24 bits contain the offset into the font's alternate data table at which alternates begin for the glyph.
			uint32				layerData;						// The high 8 bits contain the number of layers for the glyph, and the low 24 bits contain the offset into the font's layer data table at which layers begin for the glyph.
		};


		struct ExtendedGlyphData
		{
			Vector2D			glyphOffset;					// An em-space offset that is applied to the glyph. This is used when a glyph has contours identical to those of another glyph but at a different position.
			uint32				caretData;						// The high 8 bits contain the number of caret positions for the glyph, and the low 24 bits contain the offset into the font's extended data table where the positions are stored.

			const float *GetCaretPositionTable(const uint32 *extendedData) const
			{
				return (reinterpret_cast<const float *>(extendedData + (caretData & 0x00FFFFFF)));
			}
		};


		// The kern data for a particular glyph consists of an array of KernPair structures.

		struct KernPair
		{
			int32				leftIndex;						// The glyph index for the left side of the kern pair.
			float				kernValue;						// The horizontal adjustment distance, in em units, to apply to the kern pair.
		};


		// The mark data for a particular glyph consists of an array of MarkAnchor structures.

		struct MarkAnchor
		{
			int32				anchorIndex;					// The index of the anchor. This is used to match an anchor point on a mark to an anchor point on the glyph to which it's attached.
			Point2D				anchorPosition;					// The position of the anchor in em-space coordinates.
		};


		// The alternate data for a particular glyph consists of an array of AlternateGlyph structures.

		struct AlternateGlyph
		{
			uint32				alternateType;					// The type of the alternate glyph.
			int32				glyphIndex;						// The index of the alternate glyph.
		};


		// The layer data for a particular glyph consists of an array of LayerData structures.

		struct LayerData
		{
			int32				glyphIndex;						// The index of the glyph representing the layer.
			Color4U				layerColor;						// The color of the layer. If the alpha component is zero, then the current font color is used instead.
		};


		// The FontHeader structure contains general information about a font, and most of the data is used internally.
		// Most Slug library functions take a font header as the first parameter. A pointer to a FontHeader structure can be
		// obtained from the raw .slug file data by calling the GetFontHeader() function.

		struct FontHeader
		{
			uint32				fontFlags;						// Flags indicating various properties of the font.
			int32				fontKeyDataCount;				// The number of entries in the key data table.
			int32				fontKeyDataOffset;				// The offset to the key data table.

			int32				pageCount;						// The total number of 256-entry pages covered by the range of code points included in the font. Not every page must contain glyph mappings.
			int32				pageIndexOffset;				// The offset to the page index table. The page index table contains pageCount signed 16-bit entries. An entry of &minus;1 in the table indicates that a page contains no glyph mappings.
			int32				glyphIndexOffset;				// The offset to the glyph index table. There are 256 entries for each page that contains glyph mappings, and each entry is a 32-bit integer.

			int32				glyphCount;						// The total number of unique glyphs in the font.
			int32				glyphDataOffset[2];				// The offsets to the tables of GlyphData structures. The first entry is the offset to regular glyphs, and the second entry is the offset to expanded outline glyphs.

			int32				geometryDataOffset;				// The offset to the geometry data table. The location of the geometry data for a particular glyph within this table is given by the geometryData member of the GlyphData structure.
			int32				kernDataOffset;					// The offset to the kerning data table. The location of the kerning data for a particular glyph within this table is given by the kernData member of the GlyphData structure.
			int32				markDataOffset;					// The offset to the combining mark data table. The location of the combining mark data for a particular glyph within this table is given by the markData member of the GlyphData structure.
			int32				sequenceDataOffset;				// The offset to the sequence data table. The location of the sequence data for a particular glyph within this table is given by the sequenceData member of the GlyphData structure.
			int32				alternateDataOffset;			// The offset to the alternate data table. The location of the alternate data for a particular glyph within this table is given by the alternateData member of the GlyphData structure.
			int32				layerDataOffset;				// The offset to the color layer table. The location of the layer data for a particular glyph within this table is given by the layerData member of the GlyphData structure.
			int32				extendedDataOffset;				// The offset to the extended data table. The location of the extended data for a particular glyph within this table is given by the extendedData member of the GlyphData structure.

			int32				reserved[2];

			Integer2D			curveTextureSize;				// The dimensions of the texture map containing the control points for the quadratic Bézier curves composing all glyphs in the font.
			uint32				curveCompressionType;			// The compression type applied to the curve texture map.
			uint32				curveCompressedDataSize;		// The size of the compressed curve texture map data, in bytes.
			int32				curveTextureOffset;				// The offset to the compressed curve texture map data.

			Integer2D			bandTextureSize;				// The dimensions of the texture map containing the multi-band Bézier curve index data for all glyphs in the font.
			uint32				bandCompressionType;			// The compression type applied to the band texture map.
			uint32				bandCompressedDataSize;			// The size of the compressed band texture map data, in bytes.
			int32				bandTextureOffset;				// The offset to the compressed band texture map data.

			const FontKeyData *GetFontKeyDataTable(void) const
			{
				return (reinterpret_cast<const FontKeyData *>(reinterpret_cast<const char *>(this) + fontKeyDataOffset));
			}

			const int16 *GetPageIndexTable(void) const
			{
				return (reinterpret_cast<const int16 *>(reinterpret_cast<const char *>(this) + pageIndexOffset));
			}

			const int32 *GetGlyphIndexTable(int32 page) const
			{
				return (reinterpret_cast<const int32 *>(reinterpret_cast<const char *>(this) + (glyphIndexOffset + page * 1024)));
			}

			const GlyphData *GetGlyphDataTable(int32 index = 0) const
			{
				return (reinterpret_cast<const GlyphData *>(reinterpret_cast<const char *>(this) + glyphDataOffset[index]));
			}

			const float *GetGeometryDataTable(void) const
			{
				return (reinterpret_cast<const float *>(reinterpret_cast<const char *>(this) + geometryDataOffset));
			}

			const KernPair *GetKernDataTable(void) const
			{
				return (reinterpret_cast<const KernPair *>(reinterpret_cast<const char *>(this) + kernDataOffset));
			}

			const MarkAnchor *GetMarkDataTable(void) const
			{
				return (reinterpret_cast<const MarkAnchor *>(reinterpret_cast<const char *>(this) + markDataOffset));
			}

			const uint32 *GetSequenceDataTable(void) const
			{
				return (reinterpret_cast<const uint32 *>(reinterpret_cast<const char *>(this) + sequenceDataOffset));
			}

			const AlternateGlyph *GetAlternateDataTable(void) const
			{
				return (reinterpret_cast<const AlternateGlyph *>(reinterpret_cast<const char *>(this) + alternateDataOffset));
			}

			const LayerData *GetLayerDataTable(void) const
			{
				return (reinterpret_cast<const LayerData *>(reinterpret_cast<const char *>(this) + layerDataOffset));
			}

			const uint32 *GetExtendedDataTable(void) const
			{
				return (reinterpret_cast<const uint32 *>(reinterpret_cast<const char *>(this) + extendedDataOffset));
			}

			const void *GetCurveTextureImage(void) const
			{
				return (reinterpret_cast<const char *>(this) + curveTextureOffset);
			}

			const void *GetBandTextureImage(void) const
			{
				return (reinterpret_cast<const char *>(this) + bandTextureOffset);
			}
		};


		// An icon has an associated IconData structure that describes all of the icon's properties.

		struct IconData : GraphicData
		{
			uint16				colorDataLocation[2];			// The coordinates in the band texture at which color data for the icon begins, if available.
			uint32				geometryCode;					// The geometry code for the icon.

			union
			{
				Half			polygonData[16];				// Vertex coordinates for the bounding polygon.
				float			geometryData[8];
			};
		};


		struct MeshData
		{
			int32				vertexBase;
			int32				vertexCount;
			int32				triangleBase;
			int32				triangleCount;
		};


		struct ComponentData
		{
			int32				componentIndex;
			Color4U				componentColor;
			Matrix2D			componentMatrix;
			Point2D				componentPosition;
		};


		struct PictureData
		{
			uint32				pictureFlags;

			Box2D				canvasBox;
			Box2D				boundingBox;

			int32				componentCount;
			int32				componentDataOffset;
			int32				meshDataOffset;

			int32				reserved[4];

			const ComponentData *GetComponentDataTable(void) const
			{
				return (reinterpret_cast<const ComponentData *>(reinterpret_cast<const char *>(this) + componentDataOffset));
			}

			const MeshData *GetMeshDataTable(void) const
			{
				return (reinterpret_cast<const MeshData *>(reinterpret_cast<const char *>(this) + meshDataOffset));
			}
		};


		// The input to the glyph vertex shader consists of four 4D floating-point attributes and
		// one 4x8-bit unsigned integer attribute. Vertex structures are generated by the
		// BuildSlug(), BuildMultiLineText(), and AssembleSlug() functions.

		struct Vertex
		{
			Vector4D			position;						// Object-space vertex position in xy. Object-space normal direction in zw.
			Vector4D			texcoord;						// Em-space coordinates of the vertex in xy. Glyph-specific rendering data in zw.
			Vector4D			jacobian;						// 2x2 inverse Jacobian matrix relating em-space derivatives to object-space derivatives.
			Vector4D			banding;						// Band scales in xy. Band offsets in zw.
			Color4U				color;							// RGBA color of vertex.
		};


		struct MeshVertex
		{
			Vector4D			position;
			Vector4D			texcoord;
			float				jacobian;
			Color4U				color;
		};


		// Triangles generated by the BuildSlug(), BuildMultiLineText(), and AssembleSlug() functions are simply three 16-bit vertex indices.

		struct Triangle
		{
			uint16				index[3];
		};


		// The GeometryBuffer structure is passed to the BuildSlug(), BuildMultiLineText(), and AssembleSlug() functions,
		// and it contains pointers to the locations where the generated vertices and triangles are written. (This is
		// typically in GPU-visible memory that could be write-combined, so the pointers are declared volatile to ensure
		// that write order is preserved by the compiler for best performance.) When the functions return, the fields
		// of the GeometryBuffer structure have been updated to point to the end of the data that was written, and the
		// same GeometryBuffer structure could be passed to another call to BuildSlug(), BuildMultiLineText(), or
		// AssembleSlug() to append more data to the same vertex buffer.

		struct GeometryBuffer
		{
			#ifndef SLUG_NO_LEGACY

				union
				{

			#endif

			volatile Vertex		*vertexData;					// A pointer to the location where vertex data is written.

			#ifndef SLUG_NO_LEGACY

					volatile Vertex		*glyphVertex;
				};

			#endif

			#ifndef SLUG_NO_LEGACY

				union
				{

			#endif

			volatile Triangle	*triangleData;					// A pointer to the location where triangle data is written.

			#ifndef SLUG_NO_LEGACY

					volatile Triangle	*glyphTriangle;
				};

			#endif

			uint32				vertexIndex;					// The index of the first vertex written. The indices stored in the triangle data begin with this value. Unless data for multiple objects is being built in the same vertex buffer, this should be zero.
		};


		struct TextureBuffer
		{
			void				*textureData;					// A pointer to the beginning of the texture data.
			TextureType			textureType = kTextureDefault;	// The type representing the format of the texture data.
			Integer2D			textureSize;					// The width and height of the texture, in texels.
			Integer2D			writeLocation;					// The location where texture data is written.

			inline TextureBuffer() = default;
		};


		// The AlbumHeader structure contains general information about an album. A pointer to a AlbumHeader structure
		// can be obtained from the raw .slug file data by calling the GetAlbumHeader() function.

		struct AlbumHeader
		{
			uint32				albumFlags;						// Flags indicating various properties of the album.

			int32				iconCount;						// The total number of icons in the album.
			int32				iconDataOffset;					// The offset to the table of IconData structures.

			int32				pictureCount;					// The total number of pictures in the album.
			int32				pictureDataOffset;				// The offset to the table of PictureData structures.

			int32				meshVertexOffset;				// The offset to the mesh vertex data.
			int32				meshTriangleOffset;				// The offset to the mesh triangle data.

			int32				reserved[1];

			Integer2D			curveTextureSize;				// The dimensions of the texture map containing the control points for the quadratic Bézier curves composing all icons in the album.
			uint32				curveCompressionType;			// The compression type applied to the curve texture map.
			uint32				curveCompressedDataSize;		// The size of the compressed curve texture map data, in bytes.
			int32				curveTextureOffset;				// The offset to the compressed curve texture map data.

			Integer2D			bandTextureSize;				// The dimensions of the texture map containing the multi-band Bézier curve index data for all icons in the album.
			uint32				bandCompressionType;			// The compression type applied to the band texture map.
			uint32				bandCompressedDataSize;			// The size of the compressed band texture map data, in bytes.
			int32				bandTextureOffset;				// The offset to the compressed band texture map data.

			const IconData *GetIconDataTable(void) const
			{
				return (reinterpret_cast<const IconData *>(reinterpret_cast<const char *>(this) + iconDataOffset));
			}

			const PictureData *GetPictureDataTable(void) const
			{
				return (reinterpret_cast<const PictureData *>(reinterpret_cast<const char *>(this) + pictureDataOffset));
			}

			const MeshVertex *GetMeshVertexTable(void) const
			{
				return (reinterpret_cast<const MeshVertex *>(reinterpret_cast<const char *>(this) + meshVertexOffset));
			}

			const Triangle *GetMeshTriangleTable(void) const
			{
				return (reinterpret_cast<const Triangle *>(reinterpret_cast<const char *>(this) + meshTriangleOffset));
			}

			const void *GetCurveTextureImage(void) const
			{
				return (reinterpret_cast<const char *>(this) + curveTextureOffset);
			}

			const void *GetBandTextureImage(void) const
			{
				return (reinterpret_cast<const char *>(this) + bandTextureOffset);
			}
		};


		// The LineData structure contains information about a line of text.

		struct LineData
		{
			uint32				lineFlags;						// Flags indicating various properties of the line of text.

			int32				fullTextLength;					// The full length, in bytes, of the string composing the line of text.
			float				fullLineSpan;					// The physical span of the full line of text.

			int32				trimTextLength;					// The length, in bytes, of the string composing the line of text after excluding trimmed characters at the end.
			float				trimLineSpan;					// The physical span of the line of text after excluding trimmed characters at the end.
		};


		// The ColorData structure contains information about colors and gradients applied to a line of text.
		// If the gradientFlag field is false, then the second entry in the color array and both entries in the
		// gradient array are ignored. In this case, only the first entry in the color array is used, and it
		// represents the solid color with which text is rendered. If the gradientFlag field is true, then the
		// two entries of the color array correspond to colors attained at the y coordinates specified by the
		// corresponding two entries of the gradient array.

		struct ColorData
		{
			Color4U				color[2];						// The text colors. For each color, the red, green, and blue components are specified in the sRGB color space, and the alpha component is linear. If gradients are not used, then the first color is assigned to all glyph vertices, and the second color is ignored. If the gradientFlag field is true, then a gradient is applied using both colors. The alpha value should be 255 for fully opaque text. The default value for the first color is {0,0,0,255}.
			float				gradient[2];					// The y coordinates at which the gradient color equals the corresponding entry in the color field. Each y coordinate is the em-space distance above the baseline. Negative values are allowed. The two y coordinates must not be equal. This field is used only if the gradientFlag field is true.
			bool				gradientFlag;					// A flag indicating whether the color gradient is used. If this is false, then text is rendered using only color[0] as a solid color. If this is true, then text is rendered with a gradient determined by color[0] and color[1] at the y coordinates given by gradient[0] and gradient[1].
		};


		// The LayoutData structure controls everything about the appearance of a line of text. Many of these values
		// can be dynamically changed by format directives embedded in the text if the kLayoutFormatDirectives
		// bit is set in the layoutFlags field.

		struct LayoutData
		{
			uint32				fontType;							// The font type code used for multi-font layout.
			float				fontSize;							// The font size, in object space. This corresponds to the size of the em square.
			float				fontStretch;						// A stretch factor applied to the font only in the horizontal direction. This is a scale applied to each glyph as well as the advance width, tracking, and kerning distances. Set to 1.0 for no stretch.
			float				textTracking;						// The extra horizontal space added between all pairs of base glyphs, in em units. The tracking can be positive or negative. Set to 0.0 for no tracking.
			float				textSkew;							// A skew value representing the ratio of the change in x coordinate to the change in y coordinate measured upward from the baseline. Positive values cause the text to slant to the right, and negative values cause the text to slant to the left. Set to 0.0 for no skew.
			Vector2D			textScale;							// An x and y scale adjustment applied to each glyph. The font size and stretch are multiplied by this scale to obtain the final em size of the text. The scales should be positive values. Set to (1.0, 1.0) for no scale.
			Vector2D			textOffset;							// An x and y offset adjustment applied to each glyph, in em units. Positive values offset right and upward, and negative values offset left and downward. The offset is applied after the scale. Set to (0.0, 0.0) for no offset.
			ColorData			textColor;							// The solid color or gradient with which the text is rendered. See the description of the ColorData structure.

			AlignmentType		textAlignment;						// The alignment applied to multi-line text. See the descriptions of the individual values beginning with "kAlignment".
			float				textLeading;						// The amount of leading applied to multi-line text, in em units.

			float				paragraphSpacing;					// Additional spacing added between paragraphs, in em units. This field is used only if the kLayoutParagraphAttributes bit is specified in the layoutFlags field.
			float				leftMargin;							// The paragraph left margin, in absolute units. This field is used only if the kLayoutParagraphAttributes bit is specified in the layoutFlags field.
			float				rightMargin;						// The paragraph right margin, in absolute units. This field is used only if the kLayoutParagraphAttributes bit is specified in the layoutFlags field.
			float				firstLineIndent;					// The indent distance for the first line of each paragraph, in absolute units. This is added to the left or right margin, and it can be positive or negative. This field is used only if the kLayoutParagraphAttributes bit is specified in the layoutFlags field.
			float				tabSize;							// The distance between consecutive tab stops, in object space. The tab size must be greater than zero. This field is used only if the kLayoutTabSpacing bit is specified in the layoutFlags field.

			uint32				layoutFlags;						// Various flags that specify layout options. See the descriptions of the individual bits beginning with "kLayout". The default value of zero causes kerning, combining marks, sequence replacement to be enabled, and it causes format directives, clipping planes, grid mode, and paragraph attributes to be disabled.
			uint32				renderFlags;						// Various flags that specify rendering options. See the descriptions of the individual bits beginning with "kRender". The default value of zero causes all rendering optimizations for large text and special hardware capabilities to be disabled.
			GeometryType		geometryType;						// The geometry type with which glyphs are rendered. See the descriptions of the individual bits beginning with "kGeometry".

			uint32				formatMask;							// A mask that determines which embedded format directives can be applied. A one bit indicates that the corresponding format directive is enabled. See the descriptions of the individual bits beginning with "kFormat". This field is used only if the kLayoutFormatDirectives bit is specified in the layoutFlags field. The default value of ~0 enables all format directives.
			uint32				sequenceMask;						// A mask that determines which types of sequence replacement features are recognized. A one bit indicates that the corresponding type of sequence replacement is enabled. See the descriptions of the individual bits beginning with "kSequence". This field is used only if the kLayoutSequenceDisable is not specified in the layoutFlags field. The default value is the constant kSequenceDefaultMask.
			uint32				alternateMask;						// A mask that determines which types of alternate substitution features are recognized. A one bit indicates that the corresponding type of alternate substitution is enabled. See the descriptions of the individual bits beginning with "kAlternate". This field is used only if the kLayoutAlternateDisable is not specified in the layoutFlags field. The default value is zero to disable all substitutions.
			uint32				styleIndex;							// The style index used when the kAlternateStylistic bit is set in the alternateTypeMask field. This value should be in the range 1-20, and it corresponds to the style set defined by the original font. By default, this field is set to zero.
			int32				scriptLevel;						// The transform-based superscript or subscript level applied to the glyphs. Zero means no script transform, positive values correspond to superscripts, and negative values correspond to subscripts.
			bool				decorationFlag[kDecorationCount];	// An array of flags that specify which decorations to apply to the text. See the descriptions of the individual array indexes beginning with "kDecoration". By default, all decoration flags are set to false.

			EffectType			effectType;							// The type of the glyph effect to apply. Set to kEffectNone for no glyph effect.
			Vector2D			effectOffset;						// The x and y offsets at which the glyph effect is rendered, in em units. Positive values offset right and downward. This field is used only if the effectType field is not kEffectNone.
			ColorData			effectColor;						// The solid color or gradient applied to the glyph effect. See the description of the ColorData structure.

			Vector2D			objectScale;						// An x and y scale applied to the vertex positions. The scales can be positive or negative values. Set to (1.0, 1.0) for no scale.
			Vector2D			objectOffset;						// An x and y offset applied to the vertex positions, in absolute units. The offset is applied after the scale. Set to (0.0, 0.0) for no offset.

			float				clipLeft;							// The x coordinate at which glyphs are clipped on the left. This field is used only if the kLayoutClippingPlanes bit is specified in the layoutFlags field.
			float				clipRight;							// The x coordinate at which glyphs are clipped on the right. This field is used only if the kLayoutClippingPlanes bit is specified in the layoutFlags field.

			int32				missingGlyphIndex;					// The index of the glyph to be drawn when a character is missing from a font.

			uint32 GetSequenceMask(void) const
			{
				return (((layoutFlags & kLayoutSequenceDisable) == 0) ? sequenceMask : 0);
			}

			uint32 GetAlternateMask(void) const
			{
				return (((layoutFlags & kLayoutAlternateDisable) == 0) ? alternateMask : 0);
			}
		};


		struct CreateData
		{
			int32				maxBandCount;					// The maximum number of horizontal and vertical bands created for a filled path. This must be at least one and no more than kMaxFillBandCount.
			int32				maxVertexCount;					// The maximum number of vertices that may be used to form the bounding polygon for a filled path. To indicate that an optimal bounding polygon should be created, this must be in the range 4-6. To indicate that a filled path should always be rendered with a simple bounding box, this should be set to zero.
			float				interiorEdgeFactor;				// The cost factor used when considering the lengths of interior edges while searching for the optimal bounding polygon. Ignored if the maxVertexCount field is zero.
		};


		struct FillData
		{
			Color4U				fillColor;						// The color of the fill.
			GradientType		gradientType;					// The type of gradient applied to the fill. If this is kGradientNone, then no gradient is applied.

			union
			{
				Bivector3D		gradientLine;					// The scaled line corresponding to the direction and extent of a linear gradient, used when the gradientType field is kGradientLinear.
				Point3D			gradientCircle;					// The center and radius of the circle corresponding to a radial gradient, used when the gradientType field is kGradientRadial.
			};

			Color4U				gradientColor[2];				// The two colors used by the gradient.
		};


		struct StrokeData
		{
			float				strokeWidth;					// The stroke width, in object space. Half of this width lies on each side of a path.
			Color4U				strokeColor;					// The color of the stroke.

			StrokeCapType		strokeCapType;					// The type of cap applied at the beginning and end of the stroke and on the ends of dashes.
			StrokeJoinType		strokeJoinType;					// The type of join applied between consecutive curves when their tangents are not parallel and the miter limit is exceeded.
			float				miterLimit;						// The minimum ratio of the miter width to the stroke width for which consecutive curves are joined as specified by the strokeJoinType field instead of a miter. If the miter limit is zero, then curves with non-parallel tangents are always joined as specified by the strokeJoinType field.

			int32				dashCount;						// The sum of the number of dash lengths and gap lengths contained in the array specified by the dashArray field. This must be an even number less than 256. If this is not a positive number, then no dashing is applied.
			float				dashOffset;						// The length offset within the dash array at which the stroke begins. This is ignored if the dashCount field is not a positive number.
			const float			*dashArray;						// A pointer to an array of dash and gap lengths. The number of elements in this array must be equal to the value of the dashCount field. Even numbered entries specify dash lengths, and odd numbered entries specify the gap lengths between dashes. This is ignored if the dashCount field is not a positive number.
		};


		struct FontDesc
		{
			const FontHeader	*fontHeader;					// A pointer to the FontHeader structure associated with the font.
			float				fontScale;						// A scale value applied to the font. This scales all glyphs by a fixed amount, effectively multiplying the overall font size.
			float				fontOffset;						// An offset value applied to the font, in em units. This shifts all glyphs vertically by a fixed distance, where positive values offset upward. The size of the em is the product of the font size and the value of the fontScale field.

			inline FontDesc() = default;
			explicit FontDesc(const FontHeader *header, float scale = 1.0F, float offset = 0.0F) : fontHeader(header), fontScale(scale), fontOffset(offset) {}		
		};


		struct FontMap
		{
			int32				fontTypeCount;					// The number of font types. This must be at least 1.
			int32				fontSourceCount;				// The maximum number of source fonts per font type. This must be at least 1.
			const uint32		*fontTypeArray;					// A pointer to an array of application-defined font type codes. The number of elements in this array must be equal to the value of the fontTypeCount field.
			const uint8			*fontIndexTable;				// A pointer to a two-dimensional array of font indices. The number of elements in this array must be equal to the product of the values of the fontTypeCount and fontSourceCount fields.
		};


		struct TestData
		{
			bool				trailingHitFlag;				// A flag indicating whether the test position is greater than the center position of the intersected glyph.
			bool				rightToLeftFlag;				// A flag indicating whether the intersected glyph belongs to a right-to-left run of text.
			bool				subligatureFlag;				// A flag indicating that the caret position has been placed after at least one, but not all, of the characters composing a ligature glyph.

			float				positionOffset;					// The distance between the beginning position of the intersected glyph and the test position.
			float				advanceWidth;					// The advance width of the intersected glyph. If the test position is outside the text, then this is zero.
			float				caretPosition;					// The caret position derived from the test position and run direction. If the trailingHitFlag field is true, then the caret position follows the intersected glyph.

			int32				textLength;						// The length, in bytes, of the text preceding the caret position. If the trailingHitFlag field is true, then this includes the characters to which the intersected glyph corresponds.
			int32				glyphNumber;					// The number of glyphs preceding the intersected glyph. This has the same value regardless of whether the trailingHitFlag field is true.
		};


		struct CharacterData
		{
			int32				glyphIndex;						// The glyph index at the location specified by the glyphNumber parameter passed to the GetCharacterData() function.
			uint8				fontIndex;						// The index of the font from which the glyph is taken. The glyph index is valid only for this font.
			bool				forwardFlag;					// A flag indicating whether the glyph belongs to a run of text having the forward writing direction.

			int32				textLength;						// The length, in bytes, of the text preceding the first character corresponding to the glyph sequence.
			int32				characterNumber;				// The number of characters preceding the first character corresponding to the glyph sequence.
			int32				characterCount;					// The total number of characters corresponding to the glyph sequence.
			int32				glyphNumber;					// The number of glyphs preceding the first glyph in the glyph sequence.
			int32				glyphCount;						// The total number of glyphs in the glyph sequence.
		};


		struct RunData
		{
			float				runDirection;
			float				runSpan;
		};


		struct TextWorkspace
		{
			uint32				characterString[kMaxStringCharacterCount + 1];
			uint32				characterProperties[kMaxStringCharacterCount + 1];
			uint8				characterLength[kMaxStringCharacterCount + 1];

			uint32				glyphString[kMaxStringCharacterCount + 1];
			uint32				glyphProperties[kMaxStringCharacterCount + 1];

			uint8				fontIndex[kMaxStringCharacterCount + 1];
			uint8				runIndex[kMaxStringCharacterCount + 1];

			LayoutData			layoutData[kMaxStringLayoutDataCount];
			RunData				runData[kMaxStringRunDataCount];

			int32				vertexCount[kMaxStringFontCount];
			int32				triangleCount[kMaxStringFontCount];
		};


		struct FillWorkspace
		{
			QuadraticBezier2D	curveArray[kMaxFillCurveCount];
			Box2D				curveBoundingBox[kMaxFillCurveCount];
			Box2D				curveControlHull[kMaxFillCurveCount];
			uint16				curveLocation[kMaxFillCurveCount][2];
			uint16				bandCurveCount[2][kMaxFillBandCount];
			uint32				bandCurveStart[2][kMaxFillBandCount];
			Box2D				bandControlRange[kMaxFillBandCount];
			uint16				curveIndexTable[kMaxFillCurveCount * kMaxFillBandCount * 2];
		};


		struct StrokeWorkspace
		{
			float				curveLength[kMaxStrokeCurveCount];
			uint8				dashIndex[kMaxStrokeCurveCount + 1];
			float				dashOffset[kMaxStrokeCurveCount + 1];
			float				dashBase[kMaxStrokeCurveCount + 1];
		};


		struct ExtractWorkspace
		{
			uint8				textureData[((kMaxCurveTextureWidth >= kMaxBandTextureWidth) ? kMaxCurveTextureWidth : kMaxBandTextureWidth) * sizeof(Texel32)];
		};


		TERATHON_API extern const uint8 characterFlagsTable[];
		TERATHON_API extern const uint16 characterFlagsOffset[kUnicodeCharDefCount / 32];

		inline uint8 GetUnicodeCharacterFlags(uint32 unicode)
		{
			return ((unicode < kUnicodeCharDefCount) ? characterFlagsTable[characterFlagsOffset[unicode >> 5] + (unicode & 31)] : 0);
		}


		inline int32 GetFontCount(const void *slugFile)
		{
			int32 count = reinterpret_cast<const int32 *>(slugFile)[3];
			return ((count == 0) ? 1 : count);
		}

		inline const FontHeader *GetFontHeader(const void *slugFile, int32 index = 0)
		{
			return (&reinterpret_cast<const FontHeader *>(static_cast<const char *>(slugFile) + 16)[index]);
		}

		inline const AlbumHeader *GetAlbumHeader(const void *slugFile)
		{
			return (reinterpret_cast<const AlbumHeader *>(static_cast<const char *>(slugFile) + 16));
		}

		TERATHON_API const void *GetFontKeyData(const FontHeader *fontHeader, FontKeyType key);
		TERATHON_API int32 GetGlyphIndex(const FontHeader *fontHeader, uint32 unicode);

		inline const GlyphData *GetGlyphData(const FontHeader *fontHeader, uint32 unicode)
		{
			return (&fontHeader->GetGlyphDataTable()[GetGlyphIndex(fontHeader, unicode)]);
		}

		inline const IconData *GetIconData(const AlbumHeader *albumHeader, uint32 index)
		{
			return (&albumHeader->GetIconDataTable()[index]);
		}

		TERATHON_API int32 ResolveGlyph(const FontHeader *fontHeader, int32 glyphIndex, uint32 alternateMask, uint32 styleIndex = 0);
		TERATHON_API float GetKernValue(const FontHeader *fontHeader, const GlyphData *glyphData, int32 leftIndex);

		TERATHON_API void ExtractFontTextures(const FontHeader *fontHeader, void *curveTexture, void *bandTexture, ExtractWorkspace *workspace = nullptr);
		TERATHON_API void ExtractAlbumTextures(const AlbumHeader *albumHeader, void *curveTexture, void *bandTexture, ExtractWorkspace *workspace = nullptr);

		TERATHON_API void GetShaderIndices(uint32 renderFlags, uint32 *vertexIndex, uint32 *fragmentIndex);
		TERATHON_API int32 GetVertexShaderSourceCode(uint32 vertexIndex, const char **vertexCode, uint32 shaderFlags = kVertexShaderDefault);
		TERATHON_API int32 GetFragmentShaderSourceCode(uint32 fragmentIndex, const char **fragmentCode, uint32 shaderFlags = kFragmentShaderDefault);
		TERATHON_API int32 GetShaderStringLength(int32 count, const char *const *string);
		TERATHON_API void MakeShaderString(int32 count, const char *const *string, char *output);

		TERATHON_API void SetDefaultLayoutData(LayoutData *layoutData);
		TERATHON_API void UpdateLayoutData(const LayoutData *layoutData, const char *text, int32 maxLength, LayoutData *exitLayoutData);

		TERATHON_API void GetCharacterData(const TextWorkspace *workspace, uint32 glyphNumber, CharacterData *characterData);

		TERATHON_API int32 CalculateTextLengthEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, float maxSpan, float truncSpan = 0.0F, int32 *truncLength = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API float MeasureSlugEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, int32 trimCount = 0, const uint32 *trimArray = nullptr, float *trimSpan = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API void BreakSlugEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, float maxSpan, int32 softBreakCount, const uint32 *softBreakArray, int32 hardBreakCount, const uint32 *hardBreakArray, int32 trimCount, const uint32 *trimArray, LineData *lineData, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API int32 CountSlugEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, int32 *vertexCount, int32 *triangleCount, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API bool TestSlugEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, float position, TestData *testData, TextWorkspace *workspace = nullptr);
		TERATHON_API void BuildSlugEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, const Point2D& position, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr, Point2D *exitPosition = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API int32 LayoutSlugEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, const Point2D& position, uint8 *fontIndexBuffer, int32 *glyphIndexBuffer, Point2D *positionBuffer, Matrix2D *matrixBuffer, Color4U *colorBuffer, Point2D *exitPosition = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API void AssembleSlugEx(int32 fontCount, const FontDesc *fontDesc, const LayoutData *layoutData, int32 glyphCount, const uint8 *fontIndexBuffer, const int32 *glyphIndexBuffer, const Point2D *positionBuffer, const Matrix2D *matrixBuffer, const Color4U *colorBuffer, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr);
		TERATHON_API bool BuildTruncatableSlugEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, float maxSpan, const char *suffix, const Point2D& position, GeometryBuffer *geometryBuffer, Point2D *exitPosition = nullptr, TextWorkspace *workspace = nullptr);

		TERATHON_API int32 BreakMultiLineTextEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, float maxSpan, int32 softBreakCount, const uint32 *softBreakArray, int32 hardBreakCount, const uint32 *hardBreakArray, int32 trimCount, const uint32 *trimArray, int32 maxLineCount, LineData *lineDataArray, const LineData *previousLine = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API int32 CountMultiLineTextEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, int32 *vertexCount, int32 *triangleCount, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API void BuildMultiLineTextEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, const Point2D& position, float maxSpan, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
		TERATHON_API int32 LayoutMultiLineTextEx(int32 fontCount, const FontDesc *fontDesc, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, const Point2D& position, float maxSpan, uint8 *fontIndexBuffer, int32 *glyphIndexBuffer, Point2D *positionBuffer, Matrix2D *matrixBuffer, Color4U *colorBuffer, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);

		TERATHON_API uint32 BuildBoundingPolygon(int32 layerCount, const int32 *curveCount, const QuadraticBezier2D *const *curveArray, Half *polygon, int32 maxVertexCount = 4, float interiorEdgeFactor = 1.0F);
		TERATHON_API bool ImportIconData(int32 curveCount, const QuadraticBezier2D *curveArray, TextureBuffer *curveTextureBuffer, TextureBuffer *bandTextureBuffer, IconData *iconData, int32 maxVertexCount = 4, float interiorEdgeFactor = 1.0F, int32 maxBandCount = kMaxFillBandCount, FillWorkspace *workspace = nullptr);
		TERATHON_API bool ImportMulticolorIconData(int32 layerCount, const Color4U *layerColor, const int32 *curveCount, const QuadraticBezier2D *const *curveArray, TextureBuffer *curveTextureBuffer, TextureBuffer *bandTextureBuffer, IconData *iconData, int32 maxVertexCount = 4, float interiorEdgeFactor = 1.0F, int32 maxBandCount = kMaxFillBandCount, FillWorkspace *workspace = nullptr);
		TERATHON_API bool ImportPictureFillData(const FillData *fillData, int32 curveCount, const QuadraticBezier2D *curveArray, TextureBuffer *curveTextureBuffer, TextureBuffer *bandTextureBuffer, IconData *iconData, int32 maxVertexCount = 4, float interiorEdgeFactor = 1.0F, int32 maxBandCount = kMaxFillBandCount, FillWorkspace *workspace = nullptr);

		TERATHON_API void CountIcon(const IconData *iconData, GeometryType geometryType, int32 *vertexCount, int32 *triangleCount);
		TERATHON_API void BuildIcon(const IconData *iconData, const Point2D& iconPosition, const Matrix2D& iconMatrix, const ColorData *colorData, uint32 renderFlags, GeometryType geometryType, GeometryBuffer *geometryBuffer);
		TERATHON_API void CountPicture(const AlbumHeader *albumHeader, int32 pictureIndex, GeometryType geometryType, int32 *vertexCount, int32 *triangleCount);
		TERATHON_API void BuildPicture(const AlbumHeader *albumHeader, int32 pictureIndex, const Point2D& picturePosition, const Vector2D& pictureScale, GeometryType geometryType, GeometryBuffer *geometryBuffer);

		TERATHON_API void SetDefaultFillData(FillData *fillData);
		TERATHON_API void CountFill(const FillData *fillData, int32 curveCount, const QuadraticBezier2D *curveArray, const Integer2D& curveTextureSize, Integer2D *curveWriteLocation, const Integer2D& bandTextureSize, Integer2D *bandWriteLocation, int32 *vertexCount, int32 *triangleCount, const CreateData *createData = nullptr, FillWorkspace *workspace = nullptr);
		TERATHON_API void CreateFill(const FillData *fillData, int32 curveCount, const QuadraticBezier2D *curveArray, TextureBuffer *curveTextureBuffer, TextureBuffer *bandTextureBuffer, GeometryBuffer *geometryBuffer, const CreateData *createData = nullptr, FillWorkspace *workspace = nullptr);

		TERATHON_API void SetDefaultStrokeData(StrokeData *strokeData);
		TERATHON_API void CountStroke(const StrokeData *strokeData, uint32 strokeFlags, int32 curveCount, const QuadraticBezier2D *curveArray, const Integer2D& curveTextureSize, Integer2D *curveWriteLocation, int32 *vertexCount, int32 *triangleCount, StrokeWorkspace *workspace = nullptr);
		TERATHON_API void CreateStroke(const StrokeData *strokeData, uint32 strokeFlags, int32 curveCount, const QuadraticBezier2D *curveArray, TextureBuffer *curveTextureBuffer, GeometryBuffer *geometryBuffer, StrokeWorkspace *workspace = nullptr);

		TERATHON_API int32 GenerateRectangleCurves(const Point2D& pmin, const Point2D& pmax, const Vector2D& radius, QuadraticBezier2D *curveArray);
		TERATHON_API int32 GenerateCircleCurves(const Point2D& center, const Vector2D& radius, QuadraticBezier2D *curveArray);

		inline int32 CalculateTextLength(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 maxLength, float maxSpan, float truncSpan = 0.0F, int32 *truncLength = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (CalculateTextLengthEx(1, &fontDesc, nullptr, layoutData, text, maxLength, maxSpan, truncSpan, truncLength, workspace));
		}

		inline float MeasureSlug(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 maxLength, int32 trimCount = 0, const uint32 *trimArray = nullptr, float *trimSpan = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (MeasureSlugEx(1, &fontDesc, nullptr, layoutData, text, maxLength, trimCount, trimArray, trimSpan, exitLayoutData, workspace));
		}

		inline void BreakSlug(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, float maxSpan, int32 softBreakCount, const uint32 *softBreakArray, int32 hardBreakCount, const uint32 *hardBreakArray, int32 trimCount, const uint32 *trimArray, LineData *lineData, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			BreakSlugEx(1, &fontDesc, nullptr, layoutData, text, maxSpan, softBreakCount, softBreakArray, hardBreakCount, hardBreakArray, trimCount, trimArray, lineData, exitLayoutData, workspace);
		}

		inline int32 CountSlug(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 maxLength, int32 *vertexCount, int32 *triangleCount, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (CountSlugEx(1, &fontDesc, nullptr, layoutData, text, maxLength, vertexCount, triangleCount, exitLayoutData, workspace));
		}

		inline bool TestSlug(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 maxLength, float position, TestData *testData, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (TestSlugEx(1, &fontDesc, nullptr, layoutData, text, maxLength, position, testData, workspace));
		}

		inline void BuildSlug(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 maxLength, const Point2D& position, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr, Point2D *exitPosition = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			BuildSlugEx(1, &fontDesc, nullptr, layoutData, text, maxLength, position, geometryBuffer, textBox, exitPosition, exitLayoutData, workspace);
		}

		inline int32 LayoutSlug(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 maxLength, const Point2D& position, int32 *glyphIndexBuffer, Point2D *positionBuffer, Matrix2D *matrixBuffer, Color4U *colorBuffer, Point2D *exitPosition = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (LayoutSlugEx(1, &fontDesc, nullptr, layoutData, text, maxLength, position, nullptr, glyphIndexBuffer, positionBuffer, matrixBuffer, colorBuffer, exitPosition, exitLayoutData, workspace));
		}

		inline void AssembleSlug(const FontHeader *fontHeader, const LayoutData *layoutData, int32 glyphCount, const int32 *glyphIndexBuffer, const Point2D *positionBuffer, const Matrix2D *matrixBuffer, const Color4U *colorBuffer, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			AssembleSlugEx(1, &fontDesc, layoutData, glyphCount, nullptr, glyphIndexBuffer, positionBuffer, matrixBuffer, colorBuffer, geometryBuffer, textBox);
		}

		inline bool BuildTruncatableSlug(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 maxLength, float maxSpan, const char *suffix, const Point2D& position, GeometryBuffer *geometryBuffer, Point2D *exitPosition = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (BuildTruncatableSlugEx(1, &fontDesc, nullptr, layoutData, text, maxLength, maxSpan, suffix, position, geometryBuffer, exitPosition, workspace));
		}

		inline int32 BreakMultiLineText(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, float maxSpan, int32 softBreakCount, const uint32 *softBreakArray, int32 hardBreakCount, const uint32 *hardBreakArray, int32 trimCount, const uint32 *trimArray, int32 maxLineCount, LineData *lineDataArray, const LineData *previousLine = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (BreakMultiLineTextEx(1, &fontDesc, nullptr, layoutData, text, maxSpan, softBreakCount, softBreakArray, hardBreakCount, hardBreakArray, trimCount, trimArray, maxLineCount, lineDataArray, previousLine, exitLayoutData, workspace));
		}

		inline int32 CountMultiLineText(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, int32 *vertexCount, int32 *triangleCount, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (CountMultiLineTextEx(1, &fontDesc, nullptr, layoutData, text, lineIndex, lineCount, lineDataArray, vertexCount, triangleCount, exitLayoutData, workspace));
		}

		inline void BuildMultiLineText(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, const Point2D& position, float maxSpan, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			BuildMultiLineTextEx(1, &fontDesc, nullptr, layoutData, text, lineIndex, lineCount, lineDataArray, position, maxSpan, geometryBuffer, textBox, exitLayoutData, workspace);
		}

		inline int32 LayoutMultiLineText(const FontHeader *fontHeader, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, const Point2D& position, float maxSpan, int32 *glyphIndexBuffer, Point2D *positionBuffer, Matrix2D *matrixBuffer, Color4U *colorBuffer, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr)
		{
			FontDesc fontDesc(fontHeader);
			return (LayoutMultiLineTextEx(1, &fontDesc, nullptr, layoutData, text, lineIndex, lineCount, lineDataArray, position, maxSpan, nullptr, glyphIndexBuffer, positionBuffer, matrixBuffer, colorBuffer, exitLayoutData, workspace));
		}


		#ifndef SLUG_NO_LEGACY

			typedef Texel16 Texel;
			typedef Vertex GlyphVertex;
			typedef Triangle GlyphTriangle;
			typedef GeometryBuffer GlyphBuffer;
			typedef TextureBuffer TextureState;
			typedef TextWorkspace Workspace;
			typedef FillWorkspace ImportWorkspace;

			TERATHON_API int32 CalculateTextLengthEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, float maxSpan, float truncSpan = 0.0F, int32 *truncLength = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API float MeasureSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, int32 trimCount = 0, const uint32 *trimArray = nullptr, float *trimSpan = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API void BreakSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, float maxSpan, int32 softBreakCount, const uint32 *softBreakArray, int32 hardBreakCount, const uint32 *hardBreakArray, int32 trimCount, const uint32 *trimArray, LineData *lineData, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API int32 CountSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, int32 *vertexCount, int32 *triangleCount, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API bool TestSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, float position, TestData *testData, TextWorkspace *workspace = nullptr);
			TERATHON_API void BuildSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, const Point2D& position, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr, Point2D *exitPosition = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API int32 LayoutSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, const Point2D& position, uint8 *fontIndexBuffer, int32 *glyphIndexBuffer, Point2D *positionBuffer, Matrix2D *matrixBuffer, Color4U *colorBuffer, Point2D *exitPosition = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API void AssembleSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const LayoutData *layoutData, int32 glyphCount, const uint8 *fontIndexBuffer, const int32 *glyphIndexBuffer, const Point2D *positionBuffer, const Matrix2D *matrixBuffer, const Color4U *colorBuffer, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr);
			TERATHON_API bool BuildTruncatableSlugEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 maxLength, float maxSpan, const char *suffix, const Point2D& position, GeometryBuffer *geometryBuffer, Point2D *exitPosition = nullptr, TextWorkspace *workspace = nullptr);

			TERATHON_API int32 BreakMultiLineTextEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, float maxSpan, int32 softBreakCount, const uint32 *softBreakArray, int32 hardBreakCount, const uint32 *hardBreakArray, int32 trimCount, const uint32 *trimArray, int32 maxLineCount, LineData *lineDataArray, const LineData *previousLine = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API int32 CountMultiLineTextEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, int32 *vertexCount, int32 *triangleCount, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API void BuildMultiLineTextEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, const Point2D& position, float maxSpan, GeometryBuffer *geometryBuffer, Box2D *textBox = nullptr, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);
			TERATHON_API int32 LayoutMultiLineTextEx(int32 fontCount, const FontHeader *const *fontHeader, const FontMap *fontMap, const LayoutData *layoutData, const char *text, int32 lineIndex, int32 lineCount, const LineData *lineDataArray, const Point2D& position, float maxSpan, uint8 *fontIndexBuffer, int32 *glyphIndexBuffer, Point2D *positionBuffer, Matrix2D *matrixBuffer, Color4U *colorBuffer, LayoutData *exitLayoutData = nullptr, TextWorkspace *workspace = nullptr);

		#endif
	}
}


#endif
