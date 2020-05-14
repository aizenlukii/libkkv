# Overview

Note `collections` are currently marked as possible functions, but they are not
certain.

```c++
#include <string>
#include <vector>

#include <kkv/api.h>

int main() {
    std::string db_path = "path/to/db/on/disk";

    kkv::DB* db;

    kkv::Status status;
    status = kkv::DB::Open(&db, db_path);
    assert(status.ok() && status.ToString());

    status = db->Insert("key", "value");
    assert(status.ok());

    kkv::Slice buffer;
    status = db->Get("key", &buffer);
    assert(status.ok());

    const unsigned char bytes[] = { 0x00, 0xEA, 0x76, 0x61, 0x6C, 0x75, 0x65 };
    status = db->Insert("bkey", bytes);
    assert(status.ok());

    // Set the main pointer to a collection.
    // One collection may be opened only once.
    db->SetCollection("path/to/collection"); // default: /

    kkv::Wrapper wrap;
    db->PrepareWrapper(wrap);

    size_t i;
    for  (i = 0; i < 100; ++i)
        wrap.Append("key" + std::to_string(i), "value" + std::to_string(i));

    db->Insert(wrap);

    // status = db->Get("key", &buffer);
    // assert(status.ok());
    //
    // This will fail, because the key "key" doesn't exist in the
    // "/path/to/collection" collection

    // Print whole collection
    for (kkv::Pair& pair : db->Iterate())
        std::cout << pair.key << " " << pair.value << std::endl;

    // or

    wrap = kkv::Collection::Load(&db);
    for (auto& pair : wrap.Items())
        std::cout << pair.key << " " << pair.value << std::endl;

    // or (when the collection is huge)

    kkv::BufferedWrapper buf_wrap;
    buf_wrap.LoadCollection(&db);
    buf_wrap.SetBufferLimit(1024); // KiB

    while (!buf_wrap.End()) {
        buf_wrap.Load();

        for (kkv::Pair& : buf_wrap.Items())
            std::cout << pair.key << " " << pair.value << std::endl;
    }

    db->SetCollection(); // Change the collection to the default one

    kkv::Collection* col = db->GetCollection("path/to/collection");

    buf_wrap = kkv::BufferedWrapper(128);
    buf_wrap.Instance(&db);

    for (i = 0; i < 10000; ++i) {
        // Working: constantly push into the buffer when there is no more space,
        // insert the buffer into the database, release it and the element that
        // did not fit insert into the empty buffer
        buf_wrap.Append("key" + std::to_string(i), "value" + std::to_string(i));
    }


    col->Delete("key50");

    std::vector<std::string> to_delete = { "key20", "key21", "some_key" };
    // When deleting was successful, delete key from the vector.
    col->Delete(&to_delete);

    delete db;

    return 0;
}
```
