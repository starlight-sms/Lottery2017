class KeyBoard {
    status = {};

    constructor() {
        addEventListener("keydown", ev => {
            this.status[ev.key] = true;
        });

        addEventListener("keyup", ev => {
            this.status[ev.key] = false;
        });
    }

    isKeyDown(key: string) {
        return !!this.status[key];
    }
}

var kb = new KeyBoard();

export = kb;