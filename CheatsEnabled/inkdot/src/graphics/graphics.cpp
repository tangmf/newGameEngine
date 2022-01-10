// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// InkdotEngine - Graphics Handler Implementation
// ===========================================================================
// * This is a InkdotEngine component, derived from the standard GPP engine
//   templates provided by Ms. Lee Ching Yun, modified by Eugene Long.
// ===========================================================================

// include specification
#include "graphics.h"

// ===========================================================================
// default constructor
// ===========================================================================
Graphics::Graphics():

	// window states
	hwnd		(NULL),
	wndWidth	(WindowConstants::GAME_WIDTH),
	wndHeight	(WindowConstants::GAME_HEIGHT),
	fullscreen	(FALSE),
	backColor	(WindowConstants::BACK_COLOR),

	// directx interface pointers
	direct3d	(NULL),
	device3d	(NULL),
	sprite3d	(NULL),

	// buffers
	g_pVB		(NULL),

	// misc
	d3dpp		(),
	res			(E_FAIL)
{}

// ===========================================================================
// default destructor
// ===========================================================================
Graphics::~Graphics()
{
	releaseAll();
}


// methods

// ===========================================================================
// safely release all pointer referenced objects
// ===========================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}

// ===========================================================================
// initialize direct3d presentation parameters
// ===========================================================================
void Graphics::initD3Dpp()
{
	try
	{
		// initialize device presentation parameter structure
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		// configure d3dpp struct
		d3dpp.BackBufferWidth	= wndWidth;
		d3dpp.BackBufferHeight	= wndHeight;

		if (fullscreen)	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		else			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		d3dpp.BackBufferCount = 1;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = !fullscreen;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		// discard contents of the current frame buffer on page flips
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

		// turn on z-buffer
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}
	catch (...)
	{
		throw(InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initialising D3D presentation parameters!"
		));
	}
}

// ===========================================================================
// initialize graphics handler object instance
// ===========================================================================
void Graphics::initialize(
	HWND	hwnd,
	int		width,
	int		height,
	bool	fullscreen
) {
	// store function arguments into object state
	this->hwnd			= hwnd;
	this->wndWidth		= width;
	this->wndHeight		= height;
	this->fullscreen	= fullscreen;

	// initialize direct3d
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);

	// ensure d3d is initialized properly before proceeding
	if (FAILED(direct3d)) throw(
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error initializing Direct3D!"
		)
	);

	// initialize d3d presentation parameters
	initD3Dpp();

	// determine if graphics card supports necessary features like hardware
	// texturing, lighting and vertex shaders in preparation for creating a
	// d3d device.

	D3DCAPS9	caps;
	DWORD		behaviour;

	res = direct3d->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps
	);

	bool hw_supported =
		(caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1);

	// switch to software vertex processing if features are unsupported
	if (hw_supported)	behaviour = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else				behaviour = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// initialize d3d device
	res = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behaviour,
		&d3dpp,
		&device3d
	);

	// ensure d3d device is successfully initialized
	if (FAILED(res)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error creating Direct3D device!"
		)
	);

	// initialize d3d sprite interface
	res = D3DXCreateSprite(device3d, &sprite3d);

	// ensure d3d sprite interface is successfully initialized
	if (FAILED(res)) throw (
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error creating Direct3D sprite interface!"
		)
	);
}

// ===========================================================================
// clear backbuffer and begin directx scene
// ===========================================================================
HRESULT Graphics::beginSceneDraw()
{
	// default to fail, replace on success
	res = E_FAIL;

	// only draw scene if graphics device exists
	if (device3d != NULL) {

		// clear the entire screen and initialize it to backColor
		device3d->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			backColor,
			1.0f,
			0
		);

		// begin drawing scene
		res = device3d->BeginScene();

		// set view/projection transforms

		// setup view transform matrix
		D3DXMATRIX viewTransform;
		D3DXVECTOR3 eyePos	{ 0.0f, 0.0f, 10.0f };
		D3DXVECTOR3 atPos	{ 0.0f, 0.0f, 0.0f };
		D3DXVECTOR3 upPos	{ 0.0f, 1.0f, 0.0f };

		D3DXMatrixLookAtLH(
			&viewTransform,
			&eyePos,
			&atPos,
			&upPos
		);

		// apply view transform
		device3d->SetTransform(D3DTS_VIEW, &viewTransform);

		// setup projection transform matrix
		D3DXMATRIX projTransform;

		D3DXMatrixPerspectiveFovLH(
			&projTransform,
			D3DXToRadian(45),						// field of view
			(FLOAT) WindowConstants::GAME_WIDTH /	// screen aspect ratio
			(FLOAT) WindowConstants::GAME_HEIGHT,
			1.0f,									// near view-plane
			1000.0f									// far view-plane
		);

		// apply projection transform
		device3d->SetTransform(D3DTS_PROJECTION, &projTransform);
	}

	// return result
	return res;
}

// ===========================================================================
// end directx scene
// ===========================================================================
HRESULT Graphics::endSceneDraw()
{
	// default to fail, replace on success
	res = E_FAIL;

	// only end scene if graphics device exists
	if (device3d != NULL) {

		// tell device to end drawing scene
		res = device3d->EndScene();
	}

	// return result
	return res;
}

// ===========================================================================
// display offscreen buffer on the screen (page flip)
// ===========================================================================
HRESULT Graphics::showBackBuffer()
{
	// default to fail, replace on success
	res = E_FAIL;

	// Display backbuffer to screen
	res = device3d->Present(
		NULL,
		NULL,
		NULL,
		NULL
	);

	// return status
	return res;
}

// ===========================================================================
// returns the current state of the graphics device
// ===========================================================================
HRESULT Graphics::getDeviceState()
{
	// default to fail, replace on success
	res = E_FAIL;

	// get further data only if graphics device exists
	if (device3d == NULL) {

		// test if device is still bound
		res = device3d->TestCooperativeLevel();
	}

	// return result
	return res;
}

// ===========================================================================
// resets the graphics device
// ===========================================================================
HRESULT Graphics::resetDevice()
{
	// default to fail, replace on success
	res = E_FAIL;

	// re-initialise D3D presentation parameters
	initD3Dpp();

	// attempt to reset graphics device
	res = device3d->Reset(&d3dpp);

	// return result
	return res;
}

// sprite drawing

// ===========================================================================
// create and save a texture from a file to a texture object
// ===========================================================================
HRESULT Graphics::loadTexture(
	LPCWSTR			filename,
	COLOR_ARGB		transcolor,
	UINT&			textureWidth,
	UINT&			textureHeight,
	LP_TEXTURE&		texture
) {
	// init struct to read file information
	D3DXIMAGE_INFO info;

	// default to fail, replace on success
	res = E_FAIL;

	try {

		// ensure filename is provided
		if (filename == NULL) {

			// else fail gracefully and exit early
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// get width and height of texture from file
		res = D3DXGetImageInfoFromFile(filename, &info);

		// ensure info is successfully retrieved
		if (res != D3D_OK) {

			// else fail gracefully and exit early
			texture = NULL;
			return res;
		}

		// update references for texture dimensions using file info
		textureWidth	= info.Width;
		textureHeight	= info.Height;

		// create the new texture by loading data from the file
		res = D3DXCreateTextureFromFileEx(
			device3d,			// the bound d3d device interface
			filename,			// image filename
			info.Width,			// texture width
			info.Height,		// texture height
			1,					// mip-map levels (1 for no chain)
			0,					// usage (dynamic texture)
			D3DFMT_UNKNOWN,		// surface format (default)
			D3DPOOL_DEFAULT,	// memory class for texture
			D3DX_DEFAULT,		// image filter
			D3DX_DEFAULT,		// mip filter
			transcolor,			// color key for transparency
			&info,				// bitmap file info (from loaded file)
			NULL,				// color palette
			&texture			// destination texture
		);
	}
	catch (...)
	{
		// catch errors
		throw (
			InkdotError(
				InkdotErrorType::FATAL_ERROR,
				"Error in Graphics::loadTexture!"
			)
		);
	}

	// finally, return result of processing
	return res;
}

// ===========================================================================
// draws a sprite as described in a SpriteData struct
// ===========================================================================
void Graphics::drawSprite(
	const SpriteData&	spriteData,
	COLOR_ARGB			color
) {
	// ensure texture data exists for sprite
	if (spriteData.texture == NULL) return;

	// calculate center for top-left positioned sprite
	D3DXVECTOR3 center = D3DXVECTOR3(
		spriteData.width / 2 * spriteData.scale,
		spriteData.height / 2 * spriteData.scale,
		0.0f
	);

	// define translation/scaling vectors
	D3DXVECTOR3 translation = D3DXVECTOR3(
		spriteData.x, spriteData.y, NORMALIZED_Z(spriteData.z)
	);
	D3DXVECTOR3 scaling = D3DXVECTOR3(
		spriteData.scale, spriteData.scale, 0.0f
	);

	// handle flipping
	if (spriteData.flipHorizontal) {

		// reverse x-scaling to mirror entity on its left edge
		scaling.x *= -1.0f;

		// update center to flipped side
		center.x -= spriteData.width * spriteData.scale;

		// compensate for offset center by translating sprite rightwards
		translation.x += spriteData.width * spriteData.scale;
	}
	if (spriteData.flipVertical) {

		// reverse y-scaling to mirror entity on its top edge
		scaling.x *= -1.0f;

		// update center to flipped side
		center.y -= spriteData.height * spriteData.scale;

		// compensate for offset center by translating sprite downwards
		translation.y += spriteData.height * spriteData.scale;
	}

	// init rotation containers
	D3DXQUATERNION rotationQuad; D3DXMATRIX rotationMatrix;

	// calculate rotation quaternion (though we only rotate on the z-axis)
	D3DXQuaternionRotationMatrix(
		&rotationQuad,
		D3DXMatrixRotationZ(&rotationMatrix, spriteData.angle)
	);

	// init transformation matrix container
	D3DXMATRIX transformMatrix;

	// apply sprite transformation matrix (rotation, scale and position)
	sprite3d->SetTransform(D3DXMatrixTransformation(
		&transformMatrix,	// transformation matrix out
		NULL,				// keep origin at top left when scaling
		NULL,				// no scaling rotation
		&scaling,			// scale amount
		&center,			// rotation center
		&rotationQuad,		// rotation angle
		&translation		// x,y,z position
	));

	// draw sprite 
	sprite3d->Draw(
		spriteData.texture,
		&spriteData.sheetRect,
		NULL,
		NULL,
		color
	);
}

// ===========================================================================
// draws a primitive rectangle with the specified bounds
// ===========================================================================
// the vertice order & texture coords for the triangle strip are as follows:
// +----------> x   +---------> tu
// |     w          |
// |   0---1        |   0,0---1,0
// |   |  /|        |    |   / |
// | h | / |        |    |  /  |
// |   |/  |        |    | /   |
// v   2---3        v   0,1---1,1
// y                tv
// ===========================================================================
void Graphics::drawPrimitiveRect(
	const float&	x,
	const float&	y,
	const float&	z,
	const float&	w,
	const float&	h,
	COLOR_ARGB		startCol,
	COLOR_ARGB		endCol,
	bool			d
) {
	// handle single color rect
	if (!endCol) endCol = startCol;

	// normalize z value
	float nZ = NORMALIZED_Z(z);

	// set up vertex buffers
	Vertex vertices[4]
	{
		Vertex(x	, y	   , nZ, d ? startCol : startCol, 0.0f, 0.0f),
		Vertex(x + w, y	   , nZ, d ? startCol : endCol  , 1.0f, 0.0f),
		Vertex(x	, y + h, nZ, d ? endCol   : startCol, 0.0f, 1.0f),
		Vertex(x + w, y	+ h, nZ, d ? endCol   : endCol  , 1.0f, 1.0f),
		
	};

	// draw primitive
	
	drawPrimitive(vertices, 4, D3DPT_TRIANGLESTRIP, 2);
}

// ===========================================================================
// draws a primitive with the specified vertices
// ===========================================================================
void Graphics::drawPrimitive(
	Vertex				v[],
	int					nVerts,
	D3DPRIMITIVETYPE	type,
	int					primitiveCount
) {
	// create vertex buffer
	if (FAILED(device3d->CreateVertexBuffer(
		4 * sizeof(Vertex),
		D3DUSAGE_DYNAMIC,
		D3DFVF_Vertex,
		D3DPOOL_DEFAULT,
		&g_pVB,
		NULL
	))) throw(
		InkdotError(
			InkdotErrorType::FATAL_ERROR,
			"Error creating vertex buffer while drawing primitive!"
		)
	);

	// lock vertex buffer to start loading vertices
	VOID* pVertices;
	g_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD);

	// load vertex data into buffer
	memcpy(pVertices, v, nVerts*sizeof(Vertex));

	// unlock vertex buffer so it becomes usable again
	g_pVB->Unlock();

	// set stream source
	device3d->SetStreamSource(0, g_pVB, 0, sizeof(Vertex));

	// set fvf format
	device3d->SetFVF(D3DFVF_Vertex);

	// draw the primitive - draw 3
	HRESULT res = device3d->DrawPrimitive(type, 0, primitiveCount);

	// release vertex buffer memory after draw finished
	g_pVB->Release();
}
