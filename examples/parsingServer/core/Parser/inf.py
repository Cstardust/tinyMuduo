import sys
import argparse

def main():
    print("Python script is running")
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('--res_dir', type=str, help='Path to the resource directory')
    parser.add_argument('--visual', type=bool, help='Enable visual mode')
    parser.add_argument('--data_file', type=str, help='dataset path')
    parser.add_argument('--uid_store', type=str, help='uid')
    parser.add_argument('--format', type=str, help='conll')
    args = parser.parse_args(sys.argv[1:])
    print('args', args)
    print('res file', args.uid_store);
    with open(args.data_file, mode = "r") as fi, open(args.uid_store, mode = "w") as wi:
        wi.write(fi.read())

if __name__ == "__main__":
    main()
