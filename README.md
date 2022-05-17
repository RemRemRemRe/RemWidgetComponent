# Original intention 💡
Some logic could be shared between different widget blueprint, like:
- pull some text from somewhere and show it on this `TextBlock` widget

So I want to bring `Component` to `UUserWidget`, just like `Actor - Component`

Which provides an important benefit:
- layout and logic can be separate completely (all logic can be implemented in components and reused everywhere!)

# Implementation
The key problem is how to let component manipulate widgets in owner widget while they are decoupled

## BindWidget
We all know that a widget property marked with `BindWidget` or `BindWidgetOptional` will be bind to the widget with same name

By looking at its implementation (`UWidgetBlueprintGeneratedClass::InitializeWidgetStatic`),
I found it utilizing the property name to find the widget and set back to the widget property at runtime to accomplish the binding process

So if we can retrieve the member name, we could do the same process as `BindWidget` to solve this problem

## Data Structure
But how could we store the member name? Define a user struct to deal with it? like:
```C++
struct FWidgetBinding
{
    UPROPERTY()
    FName MemberName;

    UPROPERTY()
    TWeakObjectPtr<UObject> Object;
};
```
There is a better choice that do the "two things" at once : `TSoftObjectPtr`. It provides several benefits:
- It Stores object full path rather than object pointer, which allows the object address to change for being replaced or re-instanced because of recompiling or reopen the editor without losing the reference
- When the relative object get renamed, it **should** also "renamed" automatically
- `MemberName` is included in object full path
- `WeakPtr` is included as a data member
- Other engine features comes with it

but with extra overhead:
- `SubPathString` is a FString

Which looks acceptable

## Get member name at editor-time
The `FWidgetTypeCustomization` that engine provides almost do the job for me, but it has several problems:
- It only customize the raw widget object pointer ;)
- It uses the preview widget of blueprint editor as the "data source", so the reference is not persistent (reference lost after recompile)
- It does not customize things in `Instanced Objects`;) (seems like neither of the two types of detail customizations work with `Instanced Objects`)

the last two problem is "deadly" to this, because we need the component to be `instanced`, so I have to write detail customization on my own ;) :
[WidgetComponentEditor](https://github.com/RemRemRemRe/WidgetComponentEditor)

The name might be bit of misleading, it only customized the `Components` property (not a hard coded member name) of any given class at the moment, not a brand new editor :)

## Binding at runtime
@see WidgetComponentStatics::LinkSoftObjectToRuntimeVariable, what it does:
- build the `WidgetName` to `WidgetObject` map
- set the object to the `TSoftObjectPtr` with same `Name`

It should be called when widget is initialized, but this is already handled with a component~ 🍺

## Component ticking
Luckily, I avoid to reinvent the wheel with the engine provided `UUserWidgetExtension` class, it has all the interface i need,
and it is feature complete 🎉
- Initialize    (BeginPlay)
- Destruct      (EndPlay)
- RequiresTick  (IsTickEnabled)
- Tick          (Tick)

Done!

# Example Usage
## @see UComponentBasedWidget
Basically, to adopt your custom widget class with this component mechanism:
1. copy-paste all the code within this class, it does several things for you:
    - add an `UWidgetComponentAsExtension` component to your widget, it will handle most of the work for you
        - Dealing with detail customization
        - `WidgetComponentStatics::LinkSoftObjectToRuntimeVariable`
    - specify the `Components` property (the instanced components object container)
    - `WidgetComponentStatics::AddComponentsToWidgetExtension`
2. add your c++ class of the custom widget to the `UWidgetComponentEditorSetting::WidgetClassToCustomize` through project setting
3. Done!