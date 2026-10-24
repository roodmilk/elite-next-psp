/* RGB byte order matches the PSP framebuffer; immutable style parameters. */
typedef struct {unsigned ink,accent,core; float radius; unsigned char rarity;} NaturalLook;
static const NaturalLook natural_looks[RIFT_TYPE_COUNT]={
 {0x00ffb14b,0x00e755ba,0x00fcebb9,300,0},{0x0079d9f5,0x00d4d650,0x00daf8ff,300,0},{0x00ad50ed,0x00438cfa,0x00c4e7ff,300,0},{0x00ffb878,0x00e3daba,0x00fff8e0,300,0},
 {0x00d6a94a,0x00f3ec84,0x00f9ffee,440,0}, /* GLASS REEF */
 {0x008e7775,0x005b9df6,0x00aadfff,520,0}, /* SHATTERED MOONLET */
 {0x00e6b45d,0x00c6ec62,0x00fffaed,650,0}, /* COMET GARDEN */
 {0x00efa491,0x00f6b3eb,0x00fffdf5,480,0}, /* DIAMOND RAIN */
 {0x00d9e265,0x00f566a7,0x00f5ffe1,570,0}, /* MAGNETIC JELLYFISH */
 {0x00ecae3f,0x00ff66c9,0x00fff9bc,740,0}, /* PLASMA RIVER */
 {0x00f6885c,0x00ff98c6,0x00fff8ed,520,0}, /* LIGHTNING TREE */
 {0x005977f1,0x00f46c9a,0x00abe9ff,680,0}, /* FROZEN SUPERNOVA */
 {0x00debe80,0x00f4b9dd,0x00e6faf8,460,0}, /* PEARL SWARM */
 {0x004aa6ef,0x008867f7,0x00c5f7ff,580,0}, /* SOLAR LOTUS */
 {0x00caa696,0x00e9d68f,0x00c5e0ed,720,0}, /* DUST WATERFALL */
 {0x00584342,0x005377de,0x008eccfd,550,0}, /* OBSIDIAN CROWN */
 {0x00e3ca43,0x00b975f3,0x00fff6ee,640,1}, /* TWIN VORTEX */
 {0x00bca27f,0x00cfee75,0x00effae8,580,1}, /* SINGING NEEDLES */
 {0x004272ea,0x0062aaf6,0x00b9f1ff,610,1}, /* EMBER GEYSERS */
 {0x00acd15c,0x00f676a2,0x00edffcc,820,1}, /* AURORA SAIL */
 {0x00a0694d,0x0076b0e9,0x00fff0dc,460,1}, /* GRAVITATIONAL PEARL */
 {0x00ce9a67,0x0095b1f6,0x00faffed,650,1}, /* PRISM ARCH */
 {0x002b1d18,0x00b15b7c,0x00e6a1c3,770,1}, /* DARK NEBULA LUNG */
 {0x00c2cb63,0x0069abf5,0x00b5e5ff,670,1}, /* STELLAR CHRYSALIS */
 {0x00b5a293,0x00d2da6f,0x00b8e0ed,740,2}, /* SPIRAL FOSSIL */
 {0x0099583a,0x00f0956e,0x00ffefde,810,2}, /* ION THUNDERHEAD */
 {0x00d3bf4a,0x00cf7fe3,0x00f9fadb,880,2}, /* TIDAL RIBBONS */
 {0x00ba9862,0x00efdd87,0x00fffbe5,680,2}, /* WANDERING GLACIER */
 {0x00472b25,0x00ee66bf,0x00ffd3ef,760,2}, /* VOID EYE */
 {0x00b7d44d,0x00bc7ff4,0x00d9f4f7,690,2}, /* FRACTAL BLOOM */
 {0x00a28b6d,0x0083bcee,0x00f1faf6,580,2}, /* MIRROR SHOAL */
 {0x00e7915e,0x00e1e7a2,0x00ffddf8,920,2}, /* PHASE CATHEDRAL */
};
