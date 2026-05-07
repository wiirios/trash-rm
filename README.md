# trash-rm

`trash-rm` is a cli tool that moves files and directories to the system trash instead of deleting them permanently.

## Inspiration

This project is **inspired by** [trash-cli](https://github.com/andreafrancia/trash-cli), created by Andrea Francia.

The goal of `trash-rm` is not to replace the original project. Instead, it was created as a learning exercise to study the core idea behind the tool and build a new implementation from scratch based on that understanding.

## Installation
```
# Clone
git clone https://github.com/wiirios/trash-rm.git
cd trash-rm

# Install
sudo make install

# Uninstall
sudo make uninstall
```
## Usage

```
trash-rm move           trash the file.
trash-rm list           lists the files in the trash.
trash-rm match-move     move a file with a pattern.
trash-rm clean          empty the trash.
```

## Examples

Move a single file to the trash:

```bash
trash-rm move notes.txt
```

Move all files ending with .txt

```bash
trash-rm match-move *.txt
```

## TODO

Planned improvements and missing features:

- [ ] Make it cross-platform with Windows.
- [ ] Improve error handling.
- [ ] Add better logging.
- [ ] Replace repetitive code with shared utility functions

## Project status

This project is still under active development and is not recommended for production use. If you choose to try it, please restrict your tests to the `test` folder and use it with caution.

## Contributing

Contributions are welcome. If you find a bug, have an improvement idea, or want to help with documentation and features, feel free to open an issue or submit a pull request.

## MIT License
This project is licensed under MIT license, read more at <span><a href="https://docs.github.com/pt/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/licensing-a-repository">docs.github</span>
