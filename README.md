# Graph implementation in C
![example-output](https://user-images.githubusercontent.com/64109770/215203106-649a479e-c757-42c0-9acd-9bb4f719ab6c.png)

Simple graph data structure implementation in C using adjacency list. Featuring
graph output to stdout formatted in 
[dot language](https://graphviz.org/doc/info/lang.html).

## Compiling and running

```
$ make -j $(nproc)
$ ./main
```

## Generating a nice preview of the graph

First make sure you have [graphiz]() package installed, then just pipe the
output of the program to `dot`:

```
$ ./main | dot -T png -o out.png
```

this will generate a png file containing a nice representation of the graph. You
can open it with you prefered image viewer.

## License

[MIT](https://opensource.org/licenses/MIT)
