# Stage 02: Record Blocks and Catalogs

## Overview

In this stage, the focus shifts from working with raw disk blocks to understanding how a database stores and organizes relations. The database schema itself is stored on disk using two system catalogs:

- **Relation Catalog** – stores metadata about every relation in the database.
- **Attribute Catalog** – stores metadata about the attributes of each relation.

This stage introduces the concept of **record blocks**, where each block contains multiple records along with a header storing metadata such as the number of entries and links to neighboring record blocks.

---

## Objectives

- Understand the storage structure of record blocks.
- Learn the organization of the Relation Catalog and Attribute Catalog.
- Read relation and attribute metadata directly from disk.
- Display the schema of every relation in the database.
- Handle attribute catalogs spanning multiple record blocks.
- Modify catalog records to update a relation's schema.

---

## Components Implemented

### BlockBuffer

Implemented support for reading the header of a record block.

- `getHeader()`

### RecBuffer

Implemented record-level operations on record blocks.

- `getRecord()`
- `setRecord()` *(Exercise 2)*

---

## Programs Implemented

### 1. Display Database Schema

Reads the Relation Catalog and Attribute Catalog to print:

- Relation name
- Attribute names
- Attribute types

### 2. Multi-block Attribute Catalog Traversal

Extended the program to traverse multiple Attribute Catalog blocks using the `rblock` pointer in the block header, allowing schemas larger than a single block to be displayed correctly.

### 3. Schema Update

Modified the Attribute Catalog to rename the `Class` attribute of the `Students` relation to `Batch`, and verified the update by printing the schema again.

---

## Key Concepts Learned

### Record Blocks

Each record block consists of:

- Block Header
- Slot Map
- Fixed-size Record Slots

The header maintains metadata such as:

- Number of records
- Previous block (`lblock`)
- Next block (`rblock`)

allowing record blocks to be linked together.

---

### Relation Catalog

Stores metadata for every relation, including:

- Relation name
- Number of attributes
- Number of records
- First record block
- Last record block
- Number of slots

---

### Attribute Catalog

Stores metadata for every attribute of every relation, including:

- Relation name
- Attribute name
- Attribute type
- Primary flag
- Root block
- Attribute offset

---

## Learning Outcome

By the end of this stage, I understood how a relational database stores its own schema using system catalogs. I also implemented the ability to read and modify catalog records directly from disk, providing a deeper understanding of how database metadata is organized internally.