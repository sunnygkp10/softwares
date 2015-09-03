<?hh
namespace APIs\Domain\Configurations\RESTAPIs;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\Params\Param;
use APIs\Domain\Configurations\Logics\Logic;
use APIs\Domain\Configurations\Views\View;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;

interface RESTAPI {
    public function getFunctionCode(): FunctionCode;
    public function hasParams(): boolean;
    public function getParams(): ?Map<string, ?Param>;
    public function replaceParam($keyname, Param $param): this;
    public function hasSubRESTAPIs(): boolean;
    public function getSubRESTAPIs(): ?Map<string, RESTAPI>;
    public function hasSubLogics(): boolean;
    public function getSubLogics(): ?Map<string, Logic>;
    public function hasView(): boolean;
    public function getView(): ?View;
    public function hasRenderedParser(): boolean;
    public function getRenderedParser(): ?RenderedParser;
    public function hasSuccessAPI(): boolean;
    public function getSuccessAPI(): ?RESTAPI;
    public function hasFailureAPI(): boolean;
    public function getFailureAPI(): ?RESTAPI;
}
