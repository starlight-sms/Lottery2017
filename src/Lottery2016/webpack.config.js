module.exports = {
    entry: "./app/entry.ts",
    output: {
        filename: "dist.js",
        path: "./wwwroot"
    },
    resolve: {
        extensions: ["", ".ts", ".js"]
    },
    module: {
        loaders: [
            { test: /\.ts$/, loader: "awesome-typescript-loader" }
        ],
        preLoaders: [
            { test: /\.js$/, loader: "source-map-loader" }
        ]
    }
}