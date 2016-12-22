import * as $ from "jquery";

type IPromiseEventCallback<T> = (value: T) => JQueryPromise<any> | any;
type IPromiseEventCallbackVoid = () => JQueryPromise<any> | any;

abstract class PromiseEventBase<ActionType> {
    _actions = Array<ActionType>();

    connect(action: ActionType) {
        this._actions.push(action);
    }

    disconnect(action: ActionType) {
        this._actions.splice(this._actions.indexOf(action), 1);
    }
}

export class PromiseEvent<T> extends PromiseEventBase<IPromiseEventCallback<T>> {
    fire(value: T) {
        let defer = $.Deferred<void>();
        $.when(...this._actions.map(a => a(value))).then(() => {
            defer.resolve();
        }).fail(() => {
            defer.reject();
        });
        return defer.promise();
    }
}

export class PromiseEventVoid extends PromiseEventBase<IPromiseEventCallbackVoid> {
    fire() {
        let defer = $.Deferred<void>();
        $.when(...this._actions.map(a => a())).then(() => {
            defer.resolve();
        }).fail(() => {
            defer.reject();
        });
        return defer.promise();
    }
}