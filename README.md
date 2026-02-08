# C-HashMap
HashMap implementation in C

Uses the [Murmur3](https://github.com/aappleby/smhasher) hashing function, specifically a C implementation from [PeterScott](https://github.com/PeterScott/murmur3?tab=readme-ov-file).

## How to use

There is an example program in the `example/` directory. Below are instructions for using the C-Hashmap API.

### Creating the hashmap object
To create the hashmap object, use:

```C
hashmap_t* map = hashmap_create(size);
```

Where `size` is any positive integer less than 2^32. If `hashmap_create()` fails, it will return `NULL`.

### Setting a seed
To set a custom seed for the hashmap, use:

```C
hashmap_set_seed(map, seed);
```

Where `map` is your created hashmap_t* pointer and `seed` is your seed. The seed is defaulted to `0`.

### Pushing new key-value pairs
To push a new key-value pair to the map, use:

```C
error_status = hashmap_push(map, key, value);
```

Where `map` is your created hashmap_t* pointer, `key` is a string, and `value` is a pointer casted to `void *`. `hashmap_push()` will return `ERROR` if an error occured and `SUCCESS` otherwise.

### Getting a value from the hashmap
To get a value from the map with a key, use:

```C
value = hashmap_get(map, key);
```

Where `map` is your created hashmap_t* pointer and `key` is a string. `hashmap_get()` will return `NULL` if the key is not found or an error occurred, otherwise it will return the `value` associated with `key` as a `void *`. You can cast the return of `hashmap_get()` to your value's type.

### Deleting a value from the hashmap
To delete a key-value pair from the map, use:

```C
error_status = hashmap_delete(map, key, free_value_fn);
```

Where `map` is your created hashmap_t* pointer, `key` is a string, and `free_value_fn` is a function used for freeing your `value`. `free_value_fn` can be left `NULL` if your value does not need to be freed, otherwise you can pass something like `free` if it is a simple value or a custom made function for handling that. If you do create your own function for freeing your values, it should return `void` and take 1 input parameter of type `void *`. `hashmap_delete()` will return `ERROR` if an error occurs, otherwise it will return `SUCCESS`.

### Destroying a hashmap
When done, you can destroy a hashmap with:

```C
hashmap_destroy(map, free_value_fn);
```

Where `map` is your created hashmap_t* pointer, and `free_value_fn` is a function used for freeing the `values`. Same as above, `free_value_fn` can be left `NULL` if your value does not need to be freed, otherwise you can pass something like `free` if it is a simple value or a custom made function for handling that. If you do create your own function for freeing your values, it should return `void` and take 1 input parameter of type `void *`.

### Checking `errno`
This library has an `errno` that can be checked when one of the library calls fails. It is of type `hashmap_err_t` defined in the API header. To get the value of `errno`, use:

```C
hashmap_err_t error = hashmap_errno();
```

Additionally, you can get a string representation of the error with:

```C
char* error = hashmap_strerror();
```

The possible error codes and their meaning is listed below:

```
HASHMAP_ERR_NONE:               No error
HASHMAP_ERR_INVALID_CAPACITY:   An invalid capacity for the hashmap was given (from hashmap_create)
HASHMAP_ERR_NULL_ARG:           A null argument was passed into the function (from hashmap_push(), hashmap_get(), and hashmap_delete())
HASHMAP_ERR_ALLOC_FAILED:       Memory allocation failed (from hashmap_create() and hashmap_push())
HASHMAP_ERR_NOT_FOUND:          Key provided was not found in the map (from hashmap_get() and hashmap_delete())
HASHMAP_ERR_DUPLICATE:          Key provided is already in the hashmap (from hashmap_push())
```
