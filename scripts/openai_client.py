import argparse
from openai import OpenAI

client = OpenAI(
    base_url='http://localhost:11434/v1',
    api_key='ollama',  # required, but unused
)

prompt = (
    "You are a helpful assistant named echo for EchoCAD.\n"
    "Your task is to generate appropriate Python code based on the user's instructions.\n"
    "Ensure the code is compatible with PythonOCC and follows the best practices of CAD modeling.\n"
    "Always provide supplemental code based on the existing code, including necessary components\n"
    "to create and display the shape in the custom IOcctWidget viewer.\n"
    "Wrap the output code in ```python blocks."
)


def generate(command):
    response = client.chat.completions.create(
        model="llama3",
        messages=[
            {"role": "system", "content": prompt},
            {"role": "user", "content": command}
        ]
    )

    # Extract and print the generated Python code
    generated_code = response.choices[0].message.content
    return generated_code

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='EchoCAD Command Line Interface')
    parser.add_argument('command', type=str, help='Command to send to echo for generating Python code')

    args = parser.parse_args()
    generated_code = generate(args.command)

    print("Generated Python code:\n", generated_code)
