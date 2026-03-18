#!/bin/bash

{
    echo "PRAGMA journal_mode=WAL;"
    echo "PRAGMA foreign_keys=ON;"
    echo ""

    for table in city candidate vote result; do
        echo "-- $table"
        cat "../schema/tables/$table.sql"
        echo ""
    done
} > seed.sql

sqlite3 "seed.db" < seed.sql