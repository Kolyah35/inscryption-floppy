from pathlib import Path
import struct

def hashcode(s: str) -> int:
    h = 0
    for char in s:
        h = (31 * h + ord(char)) & 0xFFFFFFFF

    # convert to int32
    if h & 0x80000000:
        h -= 0x100000000
    return h

def build_pack(root: Path):
    """
    [file count (4 bytes)]

    .. each file ..
    [hash (4 bytes)] 
    [size (4 bytes)] 
    [data (size bytes)]
    """

    bytes = bytearray()
    files = root.glob("*.png")
    bytes += struct.pack("<I", len(list(files)))

    for path in root.glob("*.png"):
        pakname = path.parent.name
        
        with open(path, "rb") as f:
            raw_data = f.read()

        file_hash = hashcode(path.name)

        # int32 hash + uint32 size
        header = struct.pack("<iI", file_hash, len(raw_data))

        bytes += header
        bytes += raw_data

        print(f"Added {path} -> {pakname}.pak ({len(raw_data)} bytes)")

    return bytes

if __name__ == "__main__":
    for path in Path(".").glob("*"):
        if path.is_dir():
            data = build_pack(path)

            outname = f"{path.name}.pak"
            with open(outname, "wb") as f:
                f.write(data)
            print(f"Wrote {outname} ({len(data)} bytes)")
