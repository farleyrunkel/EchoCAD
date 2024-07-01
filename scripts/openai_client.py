import argparse
from openai import OpenAI

client = OpenAI(
    base_url='http://localhost:11434/v1',
    api_key='ollama',  # required, but unused
)

def main(user_message):
    response = client.chat.completions.create(
        model="llama3",
        messages=[
            {"role": "system", "content": "You are a helpful assistant named echo for EchoCAD. Your task is to generate appropriate Python code based on the user's instructions."},
            {"role": "user", "content": user_message}
        ]
    )

    # Extract and print the generated Python code
    generated_code = response.choices[0].message.content
    print("Generated Python code:\n", generated_code)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='EchoCAD Command Line Interface')
    parser.add_argument('command', type=str, help='Command to send to echo for generating Python code')

    args = parser.parse_args()
    main(args.command)
