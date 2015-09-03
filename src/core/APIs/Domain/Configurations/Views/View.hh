<?hh
namespace APIs\Domain\Configurations\Views;
use APIs\Domain\Configurations\Functions\FunctionCode;

interface View {
    public function getFunctionCode(): FunctionCode;
    public function hasView(): boolean;
    public function getView(): ?View;
    public function hasSubViews(): boolean;
    public function getSubViews(): ?Map<string, View>;
}
