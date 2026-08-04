# trash-rm

`trash-rm` is a cli tool that creates its own trash directory and moves files and directories there instead of deleting them permanently.

## Inspiration

This project is **inspired by** [trash-cli](https://github.com/andreafrancia/trash-cli), created by Andrea Francia.

The goal of `trash-rm` is not to replace the original project. Instead, it was created as a learning exercise to study the core idea behind the tool and build a new implementation from scratch based on that understanding.

## Requirements

- gcc >= 16.1 OR clang >= 19.1.7
- Tested on:
  - Ubunto 20.04 LTS
  - FreeBSD 15.1-RELEASE

## Installation
```
git clone https://github.com/wiirios/trash-rm.git
cd trash-rm

sudo make install

sudo make uninstall
```
## Usage

Below are all the available commands.

```
trash-rm move           trash the file.
trash-rm list           lists the files in the trash.
trash-rm match-move     move a file with a pattern.
trash-rm clean          empty the trash.
trash-rm remove         remove a file from the trash.
trash-rm recover        recovers a file from the trash.
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

Removes a specific file from the trash.

```bash
trash-rm remove min.c
```

Recover a file from the trash.

```bash
trash-rm recover bump.mp4
```

## Timestamp

`trash-rm` uses timestamps to track when files were moved to the trash. Each file in the trash receives a timestamp that is appended to its name.

### Customizing the format

To change the timestamp format, you need to modify the `TIMESTAMP_FORMAT` constant in the [include/timestamp.h](include/timestamp.h) file:

```c
// You can use any combination involving the day, month, and year.
#define TIMESTAMP_FORMAT "DDMMYYYY"
```

After changing the format, recompile the project:
```bash
sudo make uninstall
 
sudo make install
```

## TODO

Planned improvements and missing features:

- [ ] Add better logging.
- [ ] Create a function to recover all files from the Trash bin.

## Project status

This project is still under active development and is not recommended for production use. If you choose to try it, please restrict your tests to the `test` folder and use it with caution.

## Contributing

Contributions are welcome. If you find a bug, have an improvement idea, or want to help with documentation and features, feel free to open an issue or submit a pull request.

## MIT License
This project is licensed under MIT license, read more at <span><a href="https://docs.github.com/pt/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/licensing-a-repository">docs.github</span>
