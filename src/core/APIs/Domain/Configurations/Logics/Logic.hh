<?hh
namespace APIs\Domain\Configurations\Logics;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\Params\Param;

interface Logic {
    public function getFunctionCode(): FunctionCode;
    public function hasParams(): boolean;
    public function getParams(): ?Map<string, ?Param>;
    public function replaceParam($keyname, Param $param): this;
    public function hasSubLogics(): boolean;
    public function getSubLogics(): ?Map<string, Logic>;
}
