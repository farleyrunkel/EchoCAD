import echocad as echo

# Create an instance of CadModule
cad = echo.CadModule()

# Create a cube
box = cad.create_box(111, 111, 111)

# Display the cube using the viewer
echo.display(box)
