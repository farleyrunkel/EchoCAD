# module_loader.py

import sys
import os
import importlib.machinery
import importlib.util

class ModuleLoader:
    @staticmethod
    def path_import(file):
        """
        Import a module dynamically from a given file path.
        
        :param file: Path to the .pyd file
        :return: The imported module
        """
        print("\n******************* Starting Dynamic Module Loading *************************")
        
        # Details for loading the module
        loader_details = (
            importlib.machinery.ExtensionFileLoader,
            importlib.machinery.EXTENSION_SUFFIXES
        )
        
        # Create a FileFinder for the directory of the file
        tools_finder = importlib.machinery.FileFinder(os.path.dirname(file), loader_details)
        print("FileFinder: ", tools_finder)
        
        # Find the module specification
        module_name = os.path.basename(file).split(".")[0]
        toolbox_specs = tools_finder.find_spec(module_name)
        print("find_spec: ", toolbox_specs)
        
        # Create a module from the specification
        toolbox = importlib.util.module_from_spec(toolbox_specs)
        print("module: ", toolbox)
        
        # Execute the module
        toolbox_specs.loader.exec_module(toolbox)
        print("Successfully imported path_import(): ", toolbox)
        print("Check if the module is in sys.modules: ", toolbox in sys.modules)
        
        print("******************* Dynamic Module Loading Completed *************************\n")
        return toolbox

# Example usage with command-line arguments
if __name__ == "__main__":
    # Parse command-line arguments
    
    pyd_path = "E:/Projects/Project-EchoCAD/EchoCAD/out/build/x64-Debug/src/binding/echocad.cp312-win_amd64.pyd"

    # Load the module dynamically
    try:
        module = ModuleLoader.path_import(pyd_path)

        # Now you can use `module` in your code
        # For example:
        if module:
            module.test_function()  # Replace with actual function or attribute from your module

    except Exception as e:
        print(f"Error loading module: {e}")