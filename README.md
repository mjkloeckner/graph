# Graph implementation in C
![example-output](https://user-images.githubusercontent.com/64109770/215203106-649a479e-c757-42c0-9acd-9bb4f719ab6c.png)

Simple graph data structure implementation in C using adjacency list. Featuring
graph output to stdout formatted in 
[dot language](https://graphviz.org/doc/info/lang.html).

## Compiling and running

```shell
$ make -j $(nproc)
$ ./main
```

## Example output

This is the output when using the `graph_print` function:

```shell
strict graph G {
    1 -- {2, 5}
    2 -- {1, 3, 5}
    3 -- {2, 4}
    4 -- {3, 5, 6}
    5 -- {1, 2, 4}
    6 -- {4}
}
```

the output is formatted in dot language so it can be processed later by other
tools. Another function exists `graph_print_format` which does the same as
`graph_print` but adds graphviz attributes so it produces a nicer representation
of the graph when parsed with
[`dot`](https://graphviz.org/doc/info/command.html).

## Generating a preview of the graph

First make sure you have [graphviz](https://graphviz.org/) package installed,
then just pipe the output of the program to `dot`:

```shell
$ ./main | dot -T png -o out.png
```

this will generate a png file containing a nice representation of the graph. You
can open it with you preferred image viewer.

## License

[MIT](https://opensource.org/licenses/MIT)
