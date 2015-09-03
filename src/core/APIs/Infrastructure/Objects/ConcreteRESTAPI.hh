<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\RESTAPIs\RESTAPI;
use APIs\Domain\Configurations\Functions\FunctionCode;
use APIs\Domain\Configurations\Params\Param;
use APIs\Domain\Configurations\Logics\Logic;
use APIs\Domain\Configurations\Views\View;
use APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser;

final class ConcreteRESTAPI implements RESTAPI {

    private $code;
    private $params;
    private $subRESTAPIs;
    private $subLogics;
    private $view;
    private $renderedParser;
    private $successAPI;
    private $failureAPI;
    public function __construct(FunctionCode $code, Map<string, ?Param> $params = null, Map<string, RESTAPI> $subRESTAPIs = null, Map<string, Logic>  $subLogics = null, View $view = null, RenderedParser $renderedParser = null, RESTAPI $successAPI = null, RESTAPI $failureAPI = null) {

        if (empty($params)) {
            $params = null;
        }

        if (empty($subLogics)) {
            $subLogics = null;
        }

        if (!empty($successAPI) && $successAPI->hasView() && !empty($view)) {
            throw new \Exception('The successAPI and the current API cannot both have a View object.  Only 1 of these can contain a View object.');
        }
        
        $this->code = $code;
        $this->params = $params;
        $this->subRESTAPIs = $subRESTAPIs;
        $this->subLogics = $subLogics;
        $this->view = $view;
        $this->renderedParser = $renderedParser;
        $this->successAPI = $successAPI;
        $this->failureAPI = $failureAPI;

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

    public function hasSubRESTAPIs(): boolean {
        return !empty($this->subRESTAPIs);
    }

    public function getSubRESTAPIs(): ?Map<string, RESTAPI> {
        return $this->subRESTAPIs;
    }

    public function hasSubLogics(): boolean {
        return !empty($this->subLogics);
    }

    public function getSubLogics(): ?Map<string, Logic> {
        return $this->subLogics;
    }

    public function hasView(): boolean {
        return !empty($this->view);
    }

    public function getView(): ?View {
        return $this->view;
    }

    public function hasRenderedParser(): boolean {
        return !empty($this->renderedParser);
    }

    public function getRenderedParser(): ?RenderedParser {
        return $this->renderedParser;
    }

    public function hasSuccessAPI(): boolean {
        return !empty($this->successAPI);
    }

    public function getSuccessAPI(): ?RESTAPI {
        return $this->successAPI;
    }

    public function hasFailureAPI(): boolean {
        return !empty($this->failureAPI);
    }

    public function getFailureAPI(): ?RESTAPI {
        return $this->failureAPI;
    }

}
