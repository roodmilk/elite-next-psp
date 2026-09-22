param([string]$ProjectRoot = (Split-Path -Parent $PSScriptRoot))
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Drawing
$ProjectRoot = (Resolve-Path -LiteralPath $ProjectRoot).Path
$sourceDir = Join-Path $ProjectRoot 'assets/source'
$previewDir = Join-Path $ProjectRoot 'assets/preview'
$header = Join-Path $ProjectRoot 'src/next-art.h'
New-Item -ItemType Directory -Force -Path $previewDir | Out-Null

# Pure conversion: proportional grid boundaries, alpha trim where specified,
# nearest-neighbour fit, and a one-bit alpha / five-bit RGB output format.
# Generated sheets can have odd pixel dimensions. Integer grid boundaries
# distribute the extra pixel without dropping or overlapping source pixels.
$drawingReferences = @([System.Drawing.Bitmap].Assembly.Location, [System.Drawing.Rectangle].Assembly.Location) | Select-Object -Unique
$drawingReferences += @([AppDomain]::CurrentDomain.GetAssemblies() | Where-Object { $_.FullName -match '^System.Private.Windows.' } | ForEach-Object { $_.Location })
Add-Type -ReferencedAssemblies $drawingReferences -TypeDefinition @'
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
public static class NextArtCompiler {
    static Rectangle AlphaBounds(Bitmap source, Rectangle cell) {
        int left=cell.Right, top=cell.Bottom, right=-1, bottom=-1;
        for(int y=cell.Top;y<cell.Bottom;y++) for(int x=cell.Left;x<cell.Right;x++) {
            if(source.GetPixel(x,y).A<128)continue;
            if(x<left)left=x;if(x>right)right=x;
            if(y<top)top=y;if(y>bottom)bottom=y;
        }
        if(right<left||bottom<top)throw new InvalidDataException("Empty alpha tile.");
        return Rectangle.FromLTRB(left,top,right+1,bottom+1);
    }
    static Bitmap Fit(Bitmap source, Rectangle cell, int width, int height, int padding, bool trim) {
        Rectangle bounds=trim?AlphaBounds(source,cell):cell;
        if(bounds.Width<=0||bounds.Height<=0||width<=padding*2||height<=padding*2)
            throw new InvalidDataException("Invalid crop or output dimensions.");
        double scale=Math.Min((width-padding*2)/(double)bounds.Width,(height-padding*2)/(double)bounds.Height);
        int dw=Math.Max(1,(int)Math.Floor(bounds.Width*scale));
        int dh=Math.Max(1,(int)Math.Floor(bounds.Height*scale));
        int dx=(width-dw)/2,dy=(height-dh)/2;
        Bitmap output=new Bitmap(width,height,PixelFormat.Format32bppArgb);
        int opaque=0;
        for(int y=0;y<dh;y++)for(int x=0;x<dw;x++) {
            // Centre-sampled nearest neighbour; preserve hue including magenta.
            int sx=bounds.Left+(int)(((long)x*2+1)*bounds.Width/(dw*2));
            int sy=bounds.Top+(int)(((long)y*2+1)*bounds.Height/(dh*2));
            Color c=source.GetPixel(Math.Min(bounds.Right-1,sx),Math.Min(bounds.Bottom-1,sy));
            if(c.A<128)continue;
            // Preview the exact quantized runtime colours, not the source PNG.
            int r=c.R>>3,g=c.G>>3,b=c.B>>3;
            output.SetPixel(dx+x,dy+y,Color.FromArgb(255,(r<<3)|(r>>2),(g<<3)|(g>>2),(b<<3)|(b>>2)));
            opaque++;
        }
        if(opaque==0){output.Dispose();throw new InvalidDataException("Downsample produced an empty tile.");}
        return output;
    }
    static Bitmap LoadChecked(string path,int width,int height) {
        Bitmap result=new Bitmap(path);
        if(result.Width!=width||result.Height!=height) {
            result.Dispose();throw new InvalidDataException("Unexpected source dimensions: "+path+"; expected "+width+"x"+height);
        }
        if((result.PixelFormat&PixelFormat.Alpha)==0){result.Dispose();throw new InvalidDataException("Source must contain an alpha channel: "+path);}
        return result;
    }
    static Bitmap[] Grid(Bitmap image,int columns,int rows,int size,bool trim) {
        Bitmap[] result=new Bitmap[columns*rows];
        for(int y=0;y<rows;y++)for(int x=0;x<columns;x++) {
            Rectangle cell=Rectangle.FromLTRB(x*image.Width/columns,y*image.Height/rows,
                (x+1)*image.Width/columns,(y+1)*image.Height/rows);
            result[y*columns+x]=Fit(image,cell,size,size,2,trim);
        }
        return result;
    }
    static void WritePixels(StreamWriter writer,Bitmap bitmap) {
        int count=0;
        for(int y=0;y<bitmap.Height;y++)for(int x=0;x<bitmap.Width;x++) {
            Color c=bitmap.GetPixel(x,y);
            int value=c.A<128?0:0x8000|((c.R>>3)<<10)|((c.G>>3)<<5)|(c.B>>3);
            if((count&15)==0)writer.Write("  ");
            writer.Write("0x"+value.ToString("X4")+",");
            if((++count&15)==0)writer.WriteLine();
        }
        if((count&15)!=0)writer.WriteLine();
    }
    static void WriteArray(StreamWriter writer,string name,Bitmap[] images,bool matrix) {
        int count=images[0].Width*images[0].Height;
        writer.WriteLine("static const uint16_t "+name+(matrix?"["+images.Length+"]["+count+"]":"["+count+"]")+" = {");
        foreach(Bitmap image in images) {
            if(image.Width*image.Height!=count)throw new InvalidDataException("Inconsistent image size.");
            if(matrix)writer.WriteLine(" {");
            WritePixels(writer,image);
            if(matrix)writer.WriteLine(" },");
        }
        writer.WriteLine("};");
    }
    static void PreviewGrid(Bitmap[] images,int columns,int size,string path) {
        int stride=size+4;
        using(Bitmap sheet=new Bitmap(columns*stride,((images.Length+columns-1)/columns)*stride))
        using(Graphics graphics=Graphics.FromImage(sheet)) {
            graphics.Clear(Color.FromArgb(255,10,18,30));
            for(int i=0;i<images.Length;i++)graphics.DrawImageUnscaled(images[i],(i%columns)*stride+2,(i/columns)*stride+2);
            sheet.Save(path,ImageFormat.Png);
        }
    }
    public static void CompileExtras(string source,string preview,string header) {
        using(Bitmap kei=new Bitmap(Path.Combine(source,"kei-1988-source.png")))
        using(Bitmap worlds=new Bitmap(Path.Combine(source,"planets-source.png"))) {
            Bitmap[] faces=Grid(kei,4,2,64,false),faces_small=Grid(kei,4,2,32,false),planets=Grid(worlds,4,2,64,true);
            Bitmap[] factions;using(Bitmap sheet=new Bitmap(Path.Combine(source,"factions-lowres-source.png")))factions=Grid(sheet,4,2,64,false);
            Bitmap[] factions_small;using(Bitmap sheet=new Bitmap(Path.Combine(source,"factions-lowres-source.png")))factions_small=Grid(sheet,4,2,32,false);
            try {
                using(StreamWriter writer=new StreamWriter(header,false)) {
                    writer.NewLine="\n";
                    writer.WriteLine("/* Original AI art; baked ARGB1555, 196608 bytes. */\n#include <stdint.h>");
                    WriteArray(writer,"kei_faces",faces,true);
                    WriteArray(writer,"kei_faces_small",faces_small,true);
                    WriteArray(writer,"planet_sprites",planets,true);
                    WriteArray(writer,"faction_portraits",factions,true);
                    WriteArray(writer,"faction_portraits_small",factions_small,true);
                }
                PreviewGrid(factions,4,64,Path.Combine(preview,"factions-native.png"));
                PreviewGrid(factions_small,4,32,Path.Combine(preview,"factions-small-native.png"));
                PreviewGrid(faces,4,64,Path.Combine(preview,"kei-native.png"));
                PreviewGrid(planets,4,64,Path.Combine(preview,"planets-native.png"));
            } finally {foreach(Bitmap b in faces)b.Dispose();foreach(Bitmap b in faces_small)b.Dispose();foreach(Bitmap b in planets)b.Dispose();foreach(Bitmap b in factions)b.Dispose();foreach(Bitmap b in factions_small)b.Dispose();}
        }
    }
    public static void Compile(string source,string preview,string header) {
        using(Bitmap race=LoadChecked(Path.Combine(source,"next-races-source.png"),1774,887))
        using(Bitmap items=LoadChecked(Path.Combine(source,"next-items-source.png"),1402,1122))
        using(Bitmap logo=LoadChecked(Path.Combine(source,"next-logo-source.png"),2172,724)) {
            Bitmap[] portraits=Grid(race,4,2,64,false);
            Bitmap[] icons=Grid(items,5,4,32,true);
            Bitmap big=Fit(logo,new Rectangle(0,0,logo.Width,logo.Height),320,72,0,true);
            Bitmap small=Fit(logo,new Rectangle(0,0,logo.Width,logo.Height),100,18,0,true);
            try {
                using(StreamWriter writer=new StreamWriter(header,false)) {
                    writer.NewLine="\n";
                    writer.WriteLine("/* Generated by tools/bake-next-art.ps1. Original AI-generated project art. */");
                    writer.WriteLine("/* ARGB1555: bit15 opaque; R10..14 G5..9 B0..4; zero transparent. */");
                    writer.WriteLine("#ifndef ELITE_NEXT_ART_H\n#define ELITE_NEXT_ART_H\n#include <stdint.h>");
                    writer.WriteLine("#define NEXT_PORTRAIT_COUNT 8\n#define NEXT_PORTRAIT_SIZE 64\n#define NEXT_ITEM_COUNT 20\n#define NEXT_ITEM_SIZE 32");
                    writer.WriteLine("#define NEXT_LOGO_WIDTH 320\n#define NEXT_LOGO_HEIGHT 72\n#define NEXT_LOGO_SMALL_WIDTH 100\n#define NEXT_LOGO_SMALL_HEIGHT 18");
                    WriteArray(writer,"next_portraits",portraits,true);
                    WriteArray(writer,"next_items",icons,true);
                    WriteArray(writer,"next_logo",new Bitmap[]{big},false);
                    WriteArray(writer,"next_logo_small",new Bitmap[]{small},false);
                    writer.WriteLine("#endif");
                }
                PreviewGrid(portraits,4,64,Path.Combine(preview,"next-portraits-native.png"));
                PreviewGrid(icons,5,32,Path.Combine(preview,"next-items-native.png"));
                big.Save(Path.Combine(preview,"next-logo-native.png"),ImageFormat.Png);
                small.Save(Path.Combine(preview,"next-logo-small-native.png"),ImageFormat.Png);
                using(Bitmap sheet=new Bitmap(320,392))using(Graphics graphics=Graphics.FromImage(sheet)) {
                    graphics.Clear(Color.FromArgb(255,10,18,30));
                    graphics.DrawImageUnscaled(big,0,0);
                    graphics.DrawImageUnscaled(small,110,74);
                    for(int i=0;i<8;i++)graphics.DrawImageUnscaled(portraits[i],24+(i%4)*68,100+(i/4)*68);
                    for(int i=0;i<20;i++)graphics.DrawImageUnscaled(icons[i],70+(i%5)*36,242+(i/5)*36);
                    sheet.Save(Path.Combine(preview,"next-art-contact-sheet.png"),ImageFormat.Png);
                }
            } finally {
                foreach(Bitmap portrait in portraits)portrait.Dispose();
                foreach(Bitmap icon in icons)icon.Dispose();
                big.Dispose();small.Dispose();
            }
        }
    }
}
'@
[NextArtCompiler]::Compile($sourceDir, $previewDir, $header)
[NextArtCompiler]::CompileExtras($sourceDir, $previewDir, (Join-Path $ProjectRoot 'src/campaign-art.h'))
$assetRecords = @(
    @{ file='next-races-source.png'; original='exec-3bc37a71-ad83-4be1-85a5-a3682a0891bf.png'; width=1774; height=887; grid='4x2'; usage='Eight alien race portraits, row-major; retain tile composition' },
    @{ file='next-logo-source.png'; original='exec-de90aa63-06ee-4bef-86c2-7961d55446a1.png'; width=2172; height=724; grid='1x1'; usage='ELITE:NEXT logo; alpha bounds fitted without redesign' },
    @{ file='next-items-source.png'; original='exec-6b7d857c-ccb5-425d-acbc-658fcc955e55.png'; width=1402; height=1122; grid='5x4'; usage='Twenty items, row-major; each tile trimmed independently' }
)
foreach ($asset in $assetRecords) { $asset.sha256 = (Get-FileHash -LiteralPath (Join-Path $sourceDir $asset.file) -Algorithm SHA256).Hash.ToLowerInvariant() }
$provenance = [ordered]@{
    origin='Original AI-generated project art created with OpenAI image generation for this game'
    thirdPartyDownloadedAssets=$false
    conversion='Deterministic nearest-neighbour scaling, alpha threshold 128, ARGB1555; no chroma key or repainting'
    gridBoundaries='floor(column*width/columns), floor(row*height/rows); supports odd dimensions without overlap'
    sources=$assetRecords
    portraitOrder=@('human','insectoid','aquatic','reptilian','avian','crystalline','fungal','masked synthetic')
    itemOrder=@('food','textiles','radioactives','life-support capsule','liquor','luxuries','medicine','computers','machinery','alloys','firearms','furs','minerals','gold','platinum','gemstones','alien artifact','shield module','navigation computer','energy cell')
}
$provenance | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $sourceDir 'next-art-provenance.json') -Encoding UTF8
$memory = @'
ELITE:NEXT compiled art memory
Format: uint16_t ARGB1555; bit15 opaque, RGB five bits each, zero transparent.
The CPU renderer must convert this format explicitly; this is not PSP GU native ABGR1555.
Portraits: 8 x 64 x 64 x 2 = 65,536 bytes
Items: 20 x 32 x 32 x 2 = 40,960 bytes
Logo: 320 x 72 x 2 = 46,080 bytes
Small logo: 100 x 18 x 2 = 3,600 bytes
Original arrays: 156,176 bytes
Kei, planet and faction arrays: 196,608 bytes
TOTAL DECLARED SOURCE ARRAYS: 352,784 bytes
Current referenced arrays: 287,248 bytes (legacy race array is unused)
No runtime source-PNG decoding or heap allocation is needed for these arrays.
The optional host PNG previews show the exact thresholded/quantized runtime colours.
Array dimensions and source dimensions are checked by the compiler.
Rebuild: powershell -NoProfile -ExecutionPolicy Bypass -File tools/bake-next-art.ps1
'@
$memory | Set-Content -LiteralPath (Join-Path $previewDir 'next-art-memory.txt') -Encoding UTF8
Write-Output "Compiled $header"
Write-Output $memory
