import { allPerson } from "./allPerson";

export interface Person {
    Id: number;
    Name: string;
}

class Api {
    luckyPersonIds = Array<number>();

    constructor() {
        this.initialRead();
    }

    private initialRead() {
        this.luckyPersonIds = JSON.parse(localStorage.getItem("luckyPersonIds") || "[]");
    }

    private checkPoint() {
        localStorage.setItem("luckyPersonIds", JSON.stringify(this.luckyPersonIds));
    }

    getUnluckyPersons(): Person[] {
        return allPerson
            .filter(x => this.luckyPersonIds.indexOf(x.Id) === -1);
    }
}



export var api = new Api();