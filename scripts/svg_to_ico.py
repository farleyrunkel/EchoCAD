import os
from PIL import Image
import cairosvg


def svg_to_ico(svg_path, ico_path, sizes=[16, 32, 48, 64, 128, 256]):
    # Convert SVG to PNG for each specified size
    png_files = []
    for size in sizes:
        png_path = f"{svg_path}_{size}.png"
        cairosvg.svg2png(url=svg_path, write_to=png_path, output_width=size, output_height=size)
        png_files.append(png_path)
    
    # Create ICO file from PNG files
    with Image.open(png_files[0]) as img:
        img.save(ico_path, format='ICO', sizes=[(size, size) for size in sizes])
    
    # Remove temporary PNG files
    for png_file in png_files:
        os.remove(png_file)

# Usage
svg_file = '../resources/icons/EchoCAD.svg'
ico_file = '../resources/icons/EchoCAD.ico'
svg_to_ico(svg_file, ico_file)
