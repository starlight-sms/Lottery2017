export function NN<T>(t: T | null | undefined) {
    if (t === null || t === undefined) {
        throw new Error("value should never be null");
    }
    return t;
}