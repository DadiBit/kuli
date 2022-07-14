# KULI

A simple database library to store plenty of schema-less entries.

## Why?

Fast sequencial reads and writes can be useful in data lookup, especially the one
that not always has all fields defined or simply is not easy to handle with SQL databases.

## Abstract

The map file contains N chunks of 4 bytes, representing a `uint32_t` LE entry size.
The data file, on the other hand, contains the raw entry data in the form of byte "blobs".
The entry data may only be 4 GB (2^32 bytes) or less long.

## TL;DR

Schema-less entries with fast sequencial reads-n-writes; append-only.

## TODO

 - [x] Implement all functions from docs
 - [] Allow `uint16_t` sizes for 64 KB of data
 - [] Improve how files are saved (map and data in a folder?)
 - [] Test for Start Of File and End Of File on `prev`, `next` and `this`
 - [] Test data files bigger than 4 GB on machine without 64 bit support
 - [] Discuss `delete` function implementation (if/how)
 - [] Discuss `insert` function implementation (if/how)
 - [] `KULI_ASSERT` macro should use the static `kuli` variable to store information