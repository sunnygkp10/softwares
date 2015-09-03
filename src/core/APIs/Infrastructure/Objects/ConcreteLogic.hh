<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Logics\Logic;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\Params\Param;

final class ConcreteLogic implements Logic {
    private $code;
    private $params;
    private $subLogics;
    public function __construct(FunctionCode $code, Map<string, ?Param> $params = null, Map<string, Logic> $subLogics = null) {

        if (empty($subLogics)) {
            $subLogics = null;
        }

        if (empty($params)) {
            $params = null;
        }

        $this->code = $code;
        $this->params = $params;
        $this->subLogics = $subLogics;
    }

    public function getFunctionCode(): FunctionCode {
        return $this->code;
    }

    public function hasParams(): boolean {
        return !empty($this->params);
    }

    public function getParams(): ?Map<string, ?Param> {
        return $this->params;
    }

    public function replaceParam($keyname, Param $param): this {
        $this->params[$keyname] = $param;
        return $this;
    }

    public function hasSubLogics(): boolean {
        return !empty($this->subLogics);
    }

    public function getSubLogics(): ?Map<string, Logic> {
        return $this->subLogics;
    }

}
