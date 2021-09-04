# RichTextBlock Inline Decorator

## Also on the [Unreal Marketplace](https://www.unrealengine.com/marketplace/en-US/product/inline-styling-decorator-for-rich-text-block)

This is a decorator for the RichTextBlock widget in Unreal Engine version 4.20 and later.

![Demo Image](https://i.imgur.com/BJFxcN9.png)

Before Unreal 4.20, the RichTextBlock existed as an *experimental* class with slightly different functionality by default.
Developers could add **<span\> tags** inline around sections of text changing their font, size, color, and emphasis.

As of 4.20, the new RichTextBlock functions differently by default in that developers must statically pre-define all of their styles in a data table, then use inline tags to determine which style should apply to different sections of text.
This is great for defining reusable styles which can be used across a project, but not so great when the styling needs to be generated dynamically.

This plugin takes Epic's original decorator from before 4.20 and exposes it to be usable with the new 4.20 RichTextBlock. Simply install the plugin and add the decorator class to any RichTextBlock widgets that you wish to use it with.

This allows the RichTextBlock to process <span> tags as it previously did, and any Projects using the old RichTextBlock that are upgrading to 4.20 can use this to re-enable the same styling behavior.
