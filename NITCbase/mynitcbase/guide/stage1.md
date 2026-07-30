# Stage 1: Understanding the Disk Interface

**Learning Objective**

- Understand the NITCbase disk model.
- Learn how to read/write disk blocks using the `Disk` class.
- Understand how records are stored in record blocks.
- Learn the purpose of slots, catalogs and the Block Allocation Map.

---

# 1. Introduction

A **relational database** stores data in the form of **relations (tables)**.

A relation consists of:

- **Rows** → Records/Tuples
- **Columns** → Attributes
- **Domain** → Allowed values for an attribute

Since a relation can contain millions of records, scanning every record for every query is inefficient. Hence, **indexes (B+ Trees)** are used to speed up searching.

Apart from storing records, the DBMS also stores metadata about relations and attributes using:

- Relation Catalog
- Attribute Catalog

---

# 2. Disk Model

NITCbase models the disk as a sequence of blocks.

| Property | Value |
|----------|-------|
| Number of blocks | **8192** |
| Block numbers | **0 – 8191** |
| Size of one block | **2048 bytes (2 KB)** |
| Total Disk Size | **16 MB** |

---

# 3. Block Allocation Map (BMAP)

The Block Allocation Map keeps track of every block in the disk.

For every block it stores:

- Whether the block is free or occupied.
- The type of the block.

Possible block types:

- `REC` → Record Block
- `IND_INTERNAL` → Internal Index Block
- `IND_LEAF` → Leaf Index Block
- `BMAP` → Block Allocation Map

Since one byte is required per disk block,

```
8192 bytes = 4 blocks
```

Hence,

**Blocks 0–3** are reserved for storing the Block Allocation Map.

---

# 4. Reserved Blocks

| Block Number | Purpose |
|--------------|---------|
| 0 – 3 | Block Allocation Map |
| 4 | Relation Catalog |
| 5 | First Attribute Catalog Block |

When the **fdisk** command is executed,

- Blocks **0–3** are marked as `BMAP`
- Block **4** is marked as `REC`
- Block **5** is marked as `REC`

Thus, the first **6 blocks are automatically allocated**.

---

# 5. Relation & Attribute Catalogs

A **catalog** stores **metadata (data about data)**.

It does **not** store actual user records.

### Relation Catalog

Stores information about every relation.

Example:

| Relation | #Attributes | First Record Block |
|----------|-------------|--------------------|
| Student | 4 | 20 |
| Employee | 5 | 45 |

---

### Attribute Catalog

Stores information about every attribute.

Example:

| Relation | Attribute | Offset |
|----------|-----------|--------|
| Student | RollNo | 0 |
| Student | Name | 16 |
| Student | CGPA | 32 |

---

# 6. XFS Interface

The **XFS Interface** is provided to interact with `disk.xfs`.

It can be used to

- Initialize the disk
- Copy relations between host and XFS disk
- Inspect disk contents
- Verify your implementation

> **Important**
>
> Never use the XFS Interface while the NITCbase program is running.
>
> NITCbase works on a **temporary runtime copy** of the disk.
>
> The XFS Interface directly modifies the **actual disk**.
>
> Using both simultaneously can lead to inconsistencies.

---

# 7. Disk Class

The DBMS accesses the disk only through the `Disk` class.

```cpp
class Disk {
public:
    Disk();
    ~Disk();

    static int readBlock(unsigned char *block, int blockNum);
    static int writeBlock(unsigned char *block, int blockNum);
};
```

Only **one object** of this class is created at the beginning of the program.

The object exists only so that the constructor and destructor execute.

---

## Constructor

```cpp
Disk();
```

Creates a **temporary runtime copy** of the disk.

Purpose:

- Protects the original disk from accidental crashes.

---

## Destructor

```cpp
~Disk();
```

Copies all updates made during execution back to the actual disk when the program terminates normally.

---

## Static Functions

Since `readBlock()` and `writeBlock()` are static,

they are called as

```cpp
Disk::readBlock(...)
Disk::writeBlock(...)
```

instead of

```cpp
disk.readBlock(...)
```

---

# 8. Reading a Block

```cpp
unsigned char buffer[BLOCK_SIZE];

Disk::readBlock(buffer,7000);
```

### Parameters

| Parameter | Description |
|------------|-------------|
| `buffer` | Pointer to a **2048-byte** memory buffer |
| `blockNum` | Disk block number |

### Returns

- `SUCCESS`
- `E_OUTOFBOUND`

---

# 9. Writing a Block

```cpp
Disk::writeBlock(buffer,7000);
```

Writes the contents of the memory buffer back to the disk block.

Returns

- `SUCCESS`
- `E_OUTOFBOUND`

---

# 10. Example

```cpp
unsigned char buffer[BLOCK_SIZE];

Disk::readBlock(buffer,7000);

char message[]="hello";

memcpy(buffer+20,message,6);

Disk::writeBlock(buffer,7000);
```

Here,

- Block **7000** is read into memory.
- `"hello"` is copied into bytes **20–25**.
- The modified buffer is written back to disk.

To verify,

```cpp
unsigned char buffer2[BLOCK_SIZE];

Disk::readBlock(buffer2,7000);

char message2[6];

memcpy(message2,buffer2+20,6);

std::cout<<message2;
```

Output

```
hello
```

---

# 11. Record Block

A relation is stored as a **linked list of Record Blocks**.

Each Record Block contains

- Header
- Slot Map
- Actual Records

---

## Header (32 Bytes)

Stores metadata such as

- Block Type (`REC`)
- Parent Block (unused)
- Left Block
- Right Block
- Number of Records
- Number of Attributes
- Number of Slots

---

# 12. Record Size

NITCbase fixes the size of every attribute to **16 bytes**.

If a relation has **K attributes**

```
Record Size = 16 × K bytes
```

Example

| Attributes | Record Size |
|------------|-------------|
| 2 | 32 bytes |
| 4 | 64 bytes |
| 6 | 96 bytes |

---

## Why are records called variable-sized?

In NITCbase,

records are variable-sized **across relations**, because different relations can have different numbers of attributes.

Example

```
Student
3 attributes
Record Size = 48 bytes
```

```
Employee
5 attributes
Record Size = 80 bytes
```

Within the **same relation**, every record has the same size.

> In real DBMSs, record sizes can also vary within the same relation because of variable-length types like `VARCHAR` and `TEXT`.

---

# 13. Slots

Each Record Block is divided into **slots**.

Each slot stores **exactly one record**.

If a relation has **K attributes**

```
#Slots = floor(2016 / (16K + 1))
```

where

- `2016 = 2048 - 32` (space after the header)
- `+1` accounts for one byte in the Slot Map.

---

## Important

A Record Block always has **L slots**.

However,

it may contain anywhere from

```
0 to L records
```

because some slots may be empty.

---

# 14. Slot Map

Immediately after the header comes the **Slot Map**.

The Slot Map occupies **L bytes**.

Each byte represents one slot.

```
1 → SLOT_OCCUPIED

0 → SLOT_UNOCCUPIED
```

Example

```
1 1 0 1 0
```

means

```
Slot1 -> Occupied

Slot2 -> Occupied

Slot3 -> Empty

Slot4 -> Occupied

Slot5 -> Empty
```

---

# 15. Why Slots?

Without slots,

deleting one record would require shifting all following records.

Example

```
A | B | C | D

Delete B

↓

A | C | D
```

Both **C** and **D** must be moved.

---

With slots,

```
Slot1 -> A

Slot2 -> Empty

Slot3 -> C

Slot4 -> D
```

To insert a record,

- Scan the Slot Map.
- Find a free slot.
- Write the record there.
- Mark the slot as occupied.

No existing records are shifted.

> **Note:** The Slot Map still needs to be scanned to find a free slot, but scanning a few bytes is much cheaper than moving entire records.

---

# Key Takeaways

- Disk consists of **8192 blocks**, each **2048 bytes**.
- Blocks **0–3** store the Block Allocation Map.
- Block **4** stores the Relation Catalog.
- Block **5** stores the first Attribute Catalog block.
- NITCbase uses a **runtime copy** of the disk while executing.
- `Disk::readBlock()` and `Disk::writeBlock()` are the only APIs used to access the disk.
- Every relation is stored as a **linked list of Record Blocks**.
- Every attribute occupies **16 bytes** in NITCbase.
- A Record Block has a fixed number of slots (`L`), but only some may be occupied.
- The Slot Map tracks free and occupied slots, making insertion and deletion efficient.