<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Configurations\Views\View;
use APIs\Domain\Configurations\Functions\FunctionCode;

final class ConcreteView implements View {
    private $code;
    private $view;
    private $subViews;
    public function __construct(FunctionCode $code, View $view = null, Map<string, View> $subViews = null) {

        if (empty($view)) {
            $view = null;
        }

        if (empty($subViews)) {
            $subViews = null;
        }

        if (!empty($subViews)) {
            foreach($subViews as $keyname => $oneSubView) {
                if ($oneSubView->hasView()) {
                    throw new \Exception('The subViews cannot contain a View objects.  The subView with keyname ('.$keyname.') contain a View object.');
                }
            }
        }

        $this->code = $code;
        $this->view = $view;
        $this->subViews = $subViews;

    }

    public function getFunctionCode(): FunctionCode {
        return $this->code;
    }

    public function hasView(): boolean {
        return !empty($this->view);
    }

    public function getView(): ?View {
        return $this->view;
    }

    public function hasSubViews(): boolean {
        return !empty($this->subViews);
    }

    public function getSubViews(): ?Map<string, View> {
        return $this->subViews;
    }

}
