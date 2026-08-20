
UBYTE UNDOBUFFER[5];

UBYTE insert_gadSIBuff[5];
struct StringInfo insert_gadSInfo = {
	insert_gadSIBuff,	/* buffer where text will be edited */
	UNDOBUFFER,	/* optional undo buffer */
	0,	/* character position in buffer */
	5,	/* maximum number of characters to allow */
	0,	/* first displayed character buffer position */
	0,0,0,0,0,	/* Intuition initialized and maintained variables */
	0,	/* Rastport of gadget */
	0,	/* initial value for integer gadgets */
	NULL	/* alternate keymap (fill in if you set the flag) */
};

SHORT BorderVectors1[] = {
	0,0,
	54,0,
	54,10,
	0,10,
	0,0
};
struct Border Border1 = {
	-2,-1,	/* XY origin relative to container TopLeft */
	3,0,JAM1,	/* front pen, back pen and drawmode */
	5,	/* number of XY vectors */
	BorderVectors1,	/* pointer to XY vectors */
	NULL	/* next border in list */
};

struct Gadget insert_gad = {
	NULL,	/* next gadget */
	289,78,	/* origin XY of hit box relative to window TopLeft */
	51,9,	/* hit box width and height */
	NULL,	/* gadget flags */
	RELVERIFY+LONGINT,	/* activation flags */
	STRGADGET,	/* gadget type flags */
	(APTR)&Border1,	/* gadget border or image to be rendered */
	NULL,	/* alternate imagery for selection */
	NULL,	/* first IntuiText structure */
	NULL,	/* gadget mutual-exclude long word */
	(APTR)&insert_gadSInfo,	/* SpecialInfo structure */
	INSERT_GADGET,	/* user-definable data */
	NULL	/* pointer to user-definable data */
};

SHORT BorderVectors2[] = {
	0,0,
	17,0,
	17,8,
	0,8,
	0,0
};
struct Border Border2 = {
	-2,-1,	/* XY origin relative to container TopLeft */
	3,0,JAM1,	/* front pen, back pen and drawmode */
	5,	/* number of XY vectors */
	BorderVectors2,	/* pointer to XY vectors */
	NULL	/* next border in list */
};

struct IntuiText IText2 = {
	3,0,JAM2,	/* front and back text pens, drawmode and fill byte */
	-2,22,	/* XY origin relative to container TopLeft */
	NULL,	/* font pointer or NULL for default */
	"Add",	/* pointer to text */
	NULL	/* next IntuiText structure */
};

struct IntuiText IText1 = {
	3,0,JAM2,	/* front and back text pens, drawmode and fill byte */
	20,0,	/* XY origin relative to container TopLeft */
	NULL,	/* font pointer or NULL for default */
	"Delete",	/* pointer to text */
	&IText2	/* next IntuiText structure */
};

struct Gadget delete_gad = {
	&insert_gad,	/* next gadget */
	291,46,	/* origin XY of hit box relative to window TopLeft */
	14,7,	/* hit box width and height */
	NULL,	/* gadget flags */
	RELVERIFY,	/* activation flags */
	BOOLGADGET,	/* gadget type flags */
	(APTR)&Border2,	/* gadget border or image to be rendered */
	NULL,	/* alternate imagery for selection */
	&IText1,	/* first IntuiText structure */
	NULL,	/* gadget mutual-exclude long word */
	NULL,	/* SpecialInfo structure */
	DELETE_GADGET,	/* user-definable data */
	NULL	/* pointer to user-definable data */
};

struct PropInfo scroll_gadSInfo = {
	AUTOKNOB+FREEVERT,	/* PROPINFO flags */
	-1,0,	/* horizontal and vertical pot values */
	-1,3276,	/* horizontal and vertical body values */
};

struct Image Image1 = {
	0,0,	/* XY origin relative to container TopLeft */
	8,4,	/* Image width and height in pixels */
	0,	/* number of bitplanes in Image */
	NULL,	/* pointer to ImageData */
	0x0000,0x0000,	/* PlanePick and PlaneOnOff */
	NULL	/* next Image structure */
};

struct Gadget scroll_gad = {
	&delete_gad,	/* next gadget */
	264,16,	/* origin XY of hit box relative to window TopLeft */
	16,82,	/* hit box width and height */
	NULL,	/* gadget flags */
	RELVERIFY,	/* activation flags */
	PROPGADGET,	/* gadget type flags */
	(APTR)&Image1,	/* gadget border or image to be rendered */
	NULL,	/* alternate imagery for selection */
	NULL,	/* first IntuiText structure */
	NULL,	/* gadget mutual-exclude long word */
	(APTR)&scroll_gadSInfo,	/* SpecialInfo structure */
	SCROLL_BAR,	/* user-definable data */
	NULL	/* pointer to user-definable data */
};

SHORT BorderVectors3[] = {
	0,0,
	237,0,
	237,83,
	0,83,
	0,0
};
struct Border Border3 = {
	-2,-1,	/* XY origin relative to container TopLeft */
	3,0,JAM1,	/* front pen, back pen and drawmode */
	5,	/* number of XY vectors */
	BorderVectors3,	/* pointer to XY vectors */
	NULL	/* next border in list */
};

struct Gadget sel_gad = {
	&scroll_gad,	/* next gadget */
	14,16,	/* origin XY of hit box relative to window TopLeft */
	234,82,	/* hit box width and height */
	GADGHBOX+GADGHIMAGE,	/* gadget flags */
	RELVERIFY+GADGIMMEDIATE+FOLLOWMOUSE,	/* activation flags */
	BOOLGADGET,	/* gadget type flags */
	(APTR)&Border3,	/* gadget border or image to be rendered */
	NULL,	/* alternate imagery for selection */
	NULL,	/* first IntuiText structure */
	NULL,	/* gadget mutual-exclude long word */
	NULL,	/* SpecialInfo structure */
	SELECT_GADGET,	/* user-definable data */
	NULL	/* pointer to user-definable data */
};

#define GadgetList1 sel_gad

struct NewWindow NewWindowStructure1 = {
	79,30,	/* window XY origin relative to TopLeft of screen */
	374,137,	/* window width and height */
	0,1,	/* detail and block pens */
	GADGETDOWN+GADGETUP+CLOSEWINDOW+RAWKEY,	/* IDCMP flags */
	WINDOWDRAG+WINDOWDEPTH+WINDOWCLOSE+ACTIVATE,	/* other window flags */
	&sel_gad,	/* first gadget in gadget list */
	NULL,	/* custom CHECKMARK imagery */
	"Generic Selector Test",	/* window title */
	NULL,	/* custom screen pointer */
	NULL,	/* custom bitmap */
	5,5,	/* minimum width and height */
	640,200,	/* maximum width and height */
	WBENCHSCREEN	/* destination screen type */
};


/* end of PowerWindows source generation */
